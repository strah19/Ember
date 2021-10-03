/**
 * @file Mesh.cpp
 * @author strah19
 * @date October 3 2021
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file represents a Mesh object that can be part of a model.
 */

#include "Mesh.h"

namespace Ember {
    Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices) {
        this->vertices = vertices;
        this->indices = indices;
    }
}