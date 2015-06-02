//
//  Sky.h
//  Flamework
//
//  Created by Alex Hofmann on 02/06/15.
//
//

#ifndef __Flamework__Sky__
#define __Flamework__Sky__

#include <stdio.h>
#include "addendum.hpp"

class Sky
{
private:
    vmml::mat4f _modelMatrix;
    std::string _name;

public:
    Sky();
    Sky(const std::string &name);
    ~Sky();
    
    void transformModelMatrix(const vmml::mat4f &t);
    
    std::string getName() const;
    vmml::mat4f getModelMatrix() const;
};


#endif /* defined(__Flamework__Sky__) */
