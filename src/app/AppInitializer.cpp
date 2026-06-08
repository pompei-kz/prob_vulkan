//
// Created by pompei on 2026-06-08.
//

#include "app/AppInitializer.h"
#include "di/Getter.h"

app::AppInitializer::AppInitializer(getter::Getter<Settings> &setting)
    : setting_(setting)
{}
