#pragma once

#include <iostream>

#include "pez/pez.h"

#include "drawable.h"
#include "typedefs.h"
#include "vao.h"

//
// Provides a visualization for normals, helpful when debugging
//
class NormalField : Drawable {

    GLsizei _lineCount;

public:

    Vao lineBuffer;

    NormalField() : Drawable(), _lineCount(0) 
    {
    }

    // 
    // Vertex normals
    //
    NormalField(int pointComponentCount,
                const FloatList points,
                int normalComponentCount,
                const FloatList& normals, 
                float scale=1.0f) : 
        Drawable()
    {
        
        //XXX: why is this constructor in-lined?

        FloatList lines;
        lines.reserve(points.size()*2*4);

        for (unsigned ni = 0, pi = 0; 
             pi < points.size(); 
             ni += normalComponentCount,
             pi += pointComponentCount) 
        {
            _lineCount++;
             // add p0, the starting point
            for (int i = 0; i < pointComponentCount; i++) {
                lines.push_back(points[pi + i]);
            } 
            // add the homog component if we are dealing with vec3s
            if (pointComponentCount == 3) 
                lines.push_back(1.0);

            // add p1, the end point
            for (int i = 0; i < normalComponentCount; i++) {
                if (i < 3) {
                    if (0) 
                    std::cout  << points[pi + i] << " --- " 
                               << points[pi + i] + normals[ni + i]
                               << std::endl;
                    lines.push_back( points[pi + i] + (.8+.2*(rand()/float(RAND_MAX))) * scale *normals[ni + i]);
                    }
                else
                    lines.push_back(points[pi + i]);
            } 
            if (0)
            std::cout  << std::endl;

            // add the homog component if we are dealing with vec3s
            if (normalComponentCount == 3) 
                lines.push_back(1.0);
        }

        lineBuffer = Vao(4, lines);
    }

    virtual void Init();

    virtual void Draw();

    virtual void Update();

};


