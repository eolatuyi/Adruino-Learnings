/*
 * boot.h
 *
 *  Created on: Feb 24, 2017
 *      Author: ebenezer
 */

#ifndef BOOT_H_
#define BOOT_H_



#define SECTION_ATTRIBUTE(name) .name
#define STRINGIFY_SECTION_ATTRIBUTE(attribute) ##attribute
#define SECTION(name) STRINGIFY_SECTION_ATTRIBUTE(SECTION_ATTRIBUTE(name))
#define LINK(name) __attribute__ ((section (SECTION(name))))

#endif /* BOOT_H_ */
