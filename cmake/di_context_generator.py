#!/usr/bin/env python3
"""
DI Context Generator
Scans C++ header files for annotations of the form:
    /// BEAN <name> [dep_type1 dep_type2 ...]
and generates a dependency-injection context class.

The annotation must appear on the line immediately before the class opening brace,
i.e., between the 'class Name' line and '{', or immediately before the class keyword.
Both '// BEAN' and '/// BEAN' prefixes are accepted.
"""

import argparse
import re
import sys
from pathlib import Path


# ---------------------------------------------------------------------------
# Parsing
# ---------------------------------------------------------------------------

def _parse_file(header: Path, src_dir: Path) -> list[dict]:
    """Return a list of bean descriptors found in one header file."""
    rel = str(header.relative_to(src_dir)).replace("\\", "/")
    try:
        lines = header.read_text(encoding="utf-8").splitlines()
    except Exception as exc:
        print(f"aLD6sgKg8w :: Warning: cannot read {header}: {exc}", file=sys.stderr)
        return []

    depth = 0
    # Each entry: (namespace_name, depth_after_brace_opened)
    ns_stack: list[tuple[str, int]] = []
    beans: list[dict] = []

    for i, line in enumerate(lines):
        # Strip line comments for brace counting so '// {' doesn't confuse us
        clean = re.sub(r"//.*", "", line)
        opens  = clean.count("{")
        closes = clean.count("}")

        # Detect 'namespace Foo {' on this line before updating depth
        ns_match = re.match(r"\s*namespace\s+(\w+)\s*", line)
        if ns_match and opens > 0:
            ns_stack.append((ns_match.group(1), depth + opens))

        depth += opens - closes

        # Remove closed namespaces
        while ns_stack and ns_stack[-1][1] > depth:
            ns_stack.pop()

        # Look for BEAN annotation  (// BEAN or /// BEAN)
        ann = re.search(r"/{2,}\s*BEAN\s+(\S+)(.*)", line)
        if not ann:
            continue

        bean_name = ann.group(1)
        deps      = [d for d in ann.group(2).strip().split() if d]

        # Find nearest 'class ClassName' within ±3 lines
        class_name: str | None = None
        for j in range(max(0, i - 3), min(len(lines), i + 4)):
            cm = re.search(r"\bclass\s+(\w+)", lines[j])
            if cm:
                class_name = cm.group(1)
                break

        if class_name is None:
            print(f"4WYWniEyQh :: Warning: BEAN annotation at {header}:{i+1} — no 'class' found nearby, skipping",
                  file=sys.stderr)
            continue

        current_ns = "::".join(n for n, _ in ns_stack)
        full_type  = f"{current_ns}::{class_name}" if current_ns else class_name

        beans.append({
            "name":       bean_name,
            "class_type": full_type,
            "deps":       deps,
            "include":    rel,
        })

    return beans


def parse_beans(src_dir: Path) -> tuple[list[dict], dict[str, str]]:
    """Scan all .h files under src_dir. Returns (beans, type→include_path)."""
    beans: list[dict] = []
    type_to_include: dict[str, str] = {}

    for header in sorted(src_dir.rglob("*.h")):
        for b in _parse_file(header, src_dir):
            beans.append(b)
            type_to_include[b["class_type"]] = b["include"]

    return beans, type_to_include


# ---------------------------------------------------------------------------
# Code generation
# ---------------------------------------------------------------------------

def _write_if_changed(path: Path, content: str) -> None:
    if path.exists() and path.read_text(encoding="utf-8") == content:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")
    print(f"Generated: {path}")


def generate(beans: list[dict], type_to_include: dict[str, str],
             getter_class: str, getter_include: str,
             context_class: str, out_dir: Path) -> None:

    # Split 'di::Context' → namespaces=['di'], class_name='Context'
    parts         = context_class.split("::")
    ctx_name      = parts[-1]
    ctx_namespaces = parts[:-1]
    ctx_ns        = "::".join(ctx_namespaces)

    # Output file paths  (e.g.  <out_dir>/di/Context.h)
    ctx_rel  = "/".join(ctx_namespaces + [ctx_name]) if ctx_namespaces else ctx_name
    out_h    = out_dir / f"{ctx_rel}.h"
    out_cpp  = out_dir / f"{ctx_rel}.cpp"

    # Map type → bean info for dependency resolution
    type_to_bean = {b["class_type"]: b for b in beans}

    # ---- Header -------------------------------------------------------
    includes = sorted({getter_include} | {b["include"] for b in beans})

    h: list[str] = [
        "// AUTO-GENERATED — do not edit",
        "#pragma once",
    ]
    for inc in includes:
        h.append(f'#include "{inc}"')
    h += ['#include <shared_mutex>', ""]

    for ns in ctx_namespaces:
        h.append(f"namespace {ns} {{")

    h += [
        f"  class {ctx_name}",
        "  {",
        "    template <typename T> static T *finish_create(T *result)",
        "    {",
        "      if constexpr (requires { result->initialize(); }) {",
        "        result->initialize();",
        "      }",
        "      return result;",
        "    }",
        "",
        "    mutable std::shared_mutex mutex_;",
        "",
    ]

    for b in beans:
        field = f"{b['name']}_"
        h.append(
            f"    {getter_class}<{b['class_type']}> {field}"
            f"{{&mutex_, [this] {{ return create_{b['name']}(); }}}};"
        )

    h += ["", "  public:"]
    for b in beans:
        h.append(f"    {b['class_type']} *get_{b['name']}() {{ return {b['name']}_.get(); }}")

    h += ["", "  private:"]
    for b in beans:
        h.append(f"    {b['class_type']} *create_{b['name']}();")

    h.append("  };")
    for ns in reversed(ctx_namespaces):
        h.append(f"}} // namespace {ns}")
    h.append("")

    # ---- Source -------------------------------------------------------
    cpp_ctx = f"{ctx_ns}::{ctx_name}" if ctx_ns else ctx_name

    cpp: list[str] = [
        "// AUTO-GENERATED — do not edit",
        f'#include "{ctx_rel}.h"',
        "",
    ]

    for b in beans:
        dep_args = ", ".join(
            f"{type_to_bean[dep]['name']}_" if dep in type_to_bean
            else f"/* unknown dep: {dep} */"
            for dep in b["deps"]
        )
        cpp += [
            f"{b['class_type']} *{cpp_ctx}::create_{b['name']}()",
            "{",
            f"  return finish_create(new {b['class_type']}({dep_args}));",
            "}",
            "",
        ]

    _write_if_changed(out_h,   "\n".join(h))
    _write_if_changed(out_cpp, "\n".join(cpp))


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main() -> None:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--src-dir",        required=True, help="Root directory to scan for headers")
    p.add_argument("--out-dir",        required=True, help="Output directory for generated files")
    p.add_argument("--getter-class",   required=True, help="Getter template class (e.g. di::Getter)")
    p.add_argument("--getter-include", required=True, help="Include path for getter (e.g. di/Getter.h)")
    p.add_argument("--context-class",  required=True, help="Generated class name (e.g. di::Context)")
    args = p.parse_args()

    src_dir = Path(args.src_dir)
    out_dir = Path(args.out_dir)

    beans, type_to_include = parse_beans(src_dir)

    if not beans:
        print("Warning: no BEAN annotations found", file=sys.stderr)

    generate(beans, type_to_include,
             args.getter_class, args.getter_include,
             args.context_class, out_dir)


if __name__ == "__main__":
    main()
