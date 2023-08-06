#pragma once

#include "materials/mat.hpp"

namespace CT
{

inline Mat JADE{
    RGB{0.0F, 0.64F, 0.42F}  * 0.6F,  
    RGB{0.0F, 0.94F, 0.72F} * 0.5F, 
    0.95F,
    true};
inline Mat JADE2{
    RGB{0.0F, 0.31F, 0.23F}  * 0.6F,  
    RGB{0.0F, 0.5F, 0.37F}  * 0.5F, 
    0.95F,
    true};
inline Mat COPPER{
    RGB{0.64F, 0.20F, 0.08F}  * 0.6F, 
    RGB{0.94F, 0.72F, 0.21F} * 0.5F, 
    0.95F,
    true};
inline Mat REDBLACK{
    RGB{0.01F, 0.01F, 0.01F}  * 0.6F, 
    RGB{0.94F, 0.01F, 0.01F} * 0.5F, 
    0.99F,
    true};
inline Mat SILVER{
    RGB{0.51F, 0.51F, 0.51F}  * 0.6F, 
    RGB{0.51F, 0.51F, 0.51F} * 0.5F, 
    0.95F,
    true};
inline Mat MIRROR{
    RGB{0.51F, 0.51F, 0.51F}  * 0.6F, 
    RGB{0.51F, 0.51F, 0.51F} * 0.5F, 
    1.0F,
    true};
inline Mat WHITE_D{
    RGB{1.00F, 1.00F, 1.00F}, 
    RGB{1.00F, 1.00F, 1.00F}  * 0.0F, 
    0.1F,
    false};
inline Mat WHITE_S{
    RGB{1.00F, 1.00F, 1.00F}  * 0.6F, 
    RGB{1.00F, 1.00F, 1.00F}  * 0.5F, 
    0.95F,
    true};
inline Mat BLACK_D{
    RGB{0.01F, 0.01F, 0.01F}  * 0.6F, 
    RGB{0.01F, 0.01F, 0.01F}  * 0.5F, 
    0.1F,
    false};
inline Mat BLACK_S{
    RGB{0.01F, 0.01F, 0.01F}  * 0.6F, 
    RGB{0.01F, 0.01F, 0.01F}  * 0.5F, 
    0.95F,
    true};
inline Mat RED_D{
    RGB{1.0F, 0.2F, 0.2F },
    RGB{1.0F, 0.2F, 0.2F } * 0.0F,
    0.25F,
    false};
inline Mat GREEN_D{
    RGB{0.2F, 1.0F, 0.2F },
    RGB{0.2F, 1.0F, 0.2F } * 0.0F,
    0.25F,
    false};
inline Mat LIGHT{
    RGB{1.0F, 1.0F, 1.0F } * 1.0F,
    RGB{1.0F, 1.0F, 1.0F } * 1.0F,
    0.25F,
    false};

}