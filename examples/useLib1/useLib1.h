/**
 * @file useLib1.h
 * @author krokoreit (krokoreit@gmail.com)
 * @brief library to test spLogHelper 
 * @version 1.0.0
 * @date 2024-10-13
 * @copyright Copyright (c) 2024
 * 
 *  
 */

#ifndef USELIB1_H
#define USELIB1_H

#include <spLogHelper.h>


void useFunc1()
{
  spLOG_D("this is a log from inside a library function");
}


#endif // USELIB1_H
