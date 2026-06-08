#include "ContextDi.h"

app::AppInitializer *di::ContextDi::create_appInitializer()
{
  return finish_create(new app::AppInitializer(settings_, print_));
}

app::Settings *di::ContextDi::create_settings()
{
  return finish_create(new app::Settings());
}

vul::Print *di::ContextDi::create_print()
{
  return finish_create(new vul::Print());
}
