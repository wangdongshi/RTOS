/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : font06x08.c
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/25
 * Author   : WangYu
 *
 **********************************************************************/
#include "font06x08.h"

const FONT_06X08 __attribute__( ( aligned(4) ) ) sFont = { .array =
{
  6, 8,
  6,	/* AsciiCode = 0x00  [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x01 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x02 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x03 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x04 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x05 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x06 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x07 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x08 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x09 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x0a [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x0b [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x0c [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x0d [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x0e [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x0f [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  5,	/* AsciiCode = 0x10 [] */
    0x3e,
    0x22,
    0x22,
    0x3e,
    0 ,
    0 ,
  3,	/* AsciiCode = 0x11 [] */
    0x10,
    0x3e,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x12 [] */
    0x10,
    0x10,
    0x54,
    0x38,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x13 [] */
    0x10,
    0x38,
    0x54,
    0x10,
    0x10,
    0 ,
  4,	/* AsciiCode = 0x14 [] */
    0x20,
    0x20,
    0x20,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x15 [] */
    0 ,
    0xa8,
    0xa8,
    0xf8,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x16 [] */
    0 ,
    0xf8,
    0xa8,
    0xb8,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x17 [] */
    0 ,
    0xe8,
    0xa8,
    0xf8,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x18 [] */
    0 ,
    0x7c,
    0x38,
    0x10,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x19 [] */
    0 ,
    0x10,
    0x38,
    0x7c,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x1a [] */
    0x8,
    0x18,
    0x38,
    0x18,
    0x8,
    0 ,
  6,	/* AsciiCode = 0x1b [] */
    0x20,
    0x30,
    0x38,
    0x30,
    0x20,
    0 ,
  6,	/* AsciiCode = 0x1c [] */
    0xfe,
    0x8,
    0x8,
    0x10,
    0xf8,
    0 ,
  6,	/* AsciiCode = 0x1d [] */
    0x10,
    0x10,
    0x54,
    0x10,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x1e [] */
    0x10,
    0x28,
    0x54,
    0x28,
    0x44,
    0 ,
  6,	/* AsciiCode = 0x1f [] */
    0x44,
    0x28,
    0x54,
    0x28,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x20 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x21 [] */
    0 ,
    0 ,
    0xf2,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x22 [] */
    0 ,
    0xe0,
    0 ,
    0xe0,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x23 [] */
    0x28,
    0xfe,
    0x28,
    0xfe,
    0x28,
    0 ,
  6,	/* AsciiCode = 0x24 [] */
    0x24,
    0x54,
    0xfe,
    0x54,
    0x48,
    0 ,
  6,	/* AsciiCode = 0x25 [] */
    0xc4,
    0xc8,
    0x10,
    0x26,
    0x46,
    0 ,
  6,	/* AsciiCode = 0x26 [] */
    0x6c,
    0x92,
    0xaa,
    0x44,
    0xa,
    0 ,
  6,	/* AsciiCode = 0x27 [] */
    0 ,
    0xa0,
    0xc0,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x28 [] */
    0 ,
    0x38,
    0x44,
    0x82,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x29 [] */
    0 ,
    0x82,
    0x44,
    0x38,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x2a [] */
    0x28,
    0x10,
    0x7c,
    0x10,
    0x28,
    0 ,
  6,	/* AsciiCode = 0x2b [] */
    0x10,
    0x10,
    0x7c,
    0x10,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x2c [] */
    0 ,
    0xa,
    0xc,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x2d [] */
    0x10,
    0x10,
    0x10,
    0x10,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x2e [] */
    0 ,
    0x6,
    0x6,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x2f [] */
    0x4,
    0x8,
    0x10,
    0x20,
    0x40,
    0 ,
  6,	/* AsciiCode = 0x30 [] */
    0x7c,
    0x8a,
    0x92,
    0xa2,
    0x7c,
    0 ,
  6,	/* AsciiCode = 0x31 [] */
    0 ,
    0x42,
    0xfe,
    0x2,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x32 [] */
    0x42,
    0x86,
    0x8a,
    0x92,
    0x62,
    0 ,
  6,	/* AsciiCode = 0x33 [] */
    0x84,
    0x82,
    0xa2,
    0xd2,
    0x8c,
    0 ,
  6,	/* AsciiCode = 0x34 [] */
    0x18,
    0x28,
    0x48,
    0xfe,
    0x8,
    0 ,
  6,	/* AsciiCode = 0x35 [] */
    0xe4,
    0xa2,
    0xa2,
    0xa2,
    0x9c,
    0 ,
  6,	/* AsciiCode = 0x36 [] */
    0x3c,
    0x52,
    0x92,
    0x92,
    0xc,
    0 ,
  6,	/* AsciiCode = 0x37 [] */
    0x80,
    0x80,
    0x9e,
    0xa0,
    0xc0,
    0 ,
  6,	/* AsciiCode = 0x38 [] */
    0x6c,
    0x92,
    0x92,
    0x92,
    0x6c,
    0 ,
  6,	/* AsciiCode = 0x39 [] */
    0x60,
    0x92,
    0x92,
    0x94,
    0x78,
    0 ,
  6,	/* AsciiCode = 0x3a [] */
    0 ,
    0x6c,
    0x6c,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x3b [] */
    0 ,
    0x6a,
    0x6c,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x3c [] */
    0x10,
    0x28,
    0x44,
    0x82,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x3d [] */
    0x28,
    0x28,
    0x28,
    0x28,
    0x28,
    0 ,
  6,	/* AsciiCode = 0x3e [] */
    0 ,
    0x82,
    0x44,
    0x28,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x3f [] */
    0x40,
    0x80,
    0x8a,
    0x90,
    0x60,
    0 ,
  6,	/* AsciiCode = 0x40 [] */
    0x4c,
    0x92,
    0x9e,
    0x82,
    0x7c,
    0 ,
  6,	/* AsciiCode = 0x41 [] */
    0x7e,
    0x88,
    0x88,
    0x88,
    0x7e,
    0 ,
  6,	/* AsciiCode = 0x42 [] */
    0xfe,
    0x92,
    0x92,
    0x92,
    0x6c,
    0 ,
  6,	/* AsciiCode = 0x43 [] */
    0x7c,
    0x82,
    0x82,
    0x82,
    0x44,
    0 ,
  6,	/* AsciiCode = 0x44 [] */
    0xfe,
    0x82,
    0x82,
    0x44,
    0x38,
    0 ,
  6,	/* AsciiCode = 0x45 [] */
    0xfe,
    0x92,
    0x92,
    0x92,
    0x82,
    0 ,
  6,	/* AsciiCode = 0x46 [] */
    0xfe,
    0x90,
    0x90,
    0x90,
    0x80,
    0 ,
  6,	/* AsciiCode = 0x47 [] */
    0x7c,
    0x82,
    0x92,
    0x92,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x48 [] */
    0xfe,
    0x10,
    0x10,
    0x10,
    0xfe,
    0 ,
  6,	/* AsciiCode = 0x49 [] */
    0 ,
    0x82,
    0xfe,
    0x82,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x4a [] */
    0x4,
    0x2,
    0x82,
    0xfc,
    0x80,
    0 ,
  6,	/* AsciiCode = 0x4b [] */
    0xfe,
    0x10,
    0x28,
    0x44,
    0x82,
    0 ,
  6,	/* AsciiCode = 0x4c [] */
    0xfe,
    0x2,
    0x2,
    0x2,
    0x2,
    0 ,
  6,	/* AsciiCode = 0x4d [] */
    0xfe,
    0x40,
    0x30,
    0x40,
    0xfe,
    0 ,
  6,	/* AsciiCode = 0x4e [] */
    0xfe,
    0x20,
    0x10,
    0x8,
    0xfe,
    0 ,
  6,	/* AsciiCode = 0x4f [] */
    0x7c,
    0x82,
    0x82,
    0x82,
    0x7c,
    0 ,
  6,	/* AsciiCode = 0x50 [] */
    0xfe,
    0x90,
    0x90,
    0x90,
    0x60,
    0 ,
  6,	/* AsciiCode = 0x51 [] */
    0x7c,
    0x82,
    0x8a,
    0x84,
    0x7a,
    0 ,
  6,	/* AsciiCode = 0x52 [] */
    0xfe,
    0x90,
    0x98,
    0x94,
    0x62,
    0 ,
  6,	/* AsciiCode = 0x53 [] */
    0x62,
    0x92,
    0x92,
    0x92,
    0x8c,
    0 ,
  6,	/* AsciiCode = 0x54 [] */
    0x80,
    0x80,
    0xfe,
    0x80,
    0x80,
    0 ,
  6,	/* AsciiCode = 0x55 [] */
    0xfc,
    0x2,
    0x2,
    0x2,
    0xfc,
    0 ,
  6,	/* AsciiCode = 0x56 [] */
    0xf8,
    0x4,
    0x2,
    0x4,
    0xf8,
    0 ,
  6,	/* AsciiCode = 0x57 [] */
    0xfc,
    0x2,
    0x1c,
    0x2,
    0xfc,
    0 ,
  6,	/* AsciiCode = 0x58 [] */
    0xc6,
    0x28,
    0x10,
    0x28,
    0xc6,
    0 ,
  6,	/* AsciiCode = 0x59 [] */
    0xe0,
    0x10,
    0xe,
    0x10,
    0xe0,
    0 ,
  6,	/* AsciiCode = 0x5a [] */
    0x86,
    0x8a,
    0x92,
    0xa2,
    0xc2,
    0 ,
  6,	/* AsciiCode = 0x5b [] */
    0 ,
    0xfe,
    0x82,
    0x82,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x5c [] */
    0xa8,
    0x68,
    0x3e,
    0x68,
    0xa8,
    0 ,
  6,	/* AsciiCode = 0x5d [] */
    0 ,
    0x82,
    0x82,
    0xfe,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x5e [] */
    0x20,
    0x40,
    0x80,
    0x40,
    0x20,
    0 ,
  6,	/* AsciiCode = 0x5f [] */
    0x1,
    0x1,
    0x1,
    0x1,
    0x1,
    0 ,
  6,	/* AsciiCode = 0x60 [] */
    0 ,
    0x80,
    0x40,
    0x20,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x61 [] */
    0x4,
    0x2a,
    0x2a,
    0x2a,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x62 [] */
    0xfe,
    0x12,
    0x22,
    0x22,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0x63 [] */
    0x1c,
    0x22,
    0x22,
    0x22,
    0x4,
    0 ,
  6,	/* AsciiCode = 0x64 [] */
    0x1c,
    0x22,
    0x22,
    0x12,
    0xfe,
    0 ,
  6,	/* AsciiCode = 0x65 [] */
    0x1c,
    0x2a,
    0x2a,
    0x2a,
    0x18,
    0 ,
  6,	/* AsciiCode = 0x66 [] */
    0x10,
    0x7e,
    0x90,
    0x80,
    0x40,
    0 ,
  6,	/* AsciiCode = 0x67 [] */
    0x30,
    0x4a,
    0x4a,
    0x4a,
    0x7c,
    0 ,
  6,	/* AsciiCode = 0x68 [] */
    0xfe,
    0x10,
    0x20,
    0x20,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x69 [] */
    0 ,
    0x22,
    0xbe,
    0x2,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x6a [] */
    0x4,
    0x2,
    0x22,
    0xbc,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x6b [] */
    0xfe,
    0x8,
    0x14,
    0x22,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x6c [] */
    0 ,
    0x82,
    0xfe,
    0x2,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x6d [] */
    0x3e,
    0x20,
    0x18,
    0x20,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x6e [] */
    0x3e,
    0x10,
    0x20,
    0x20,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x6f [] */
    0x1c,
    0x22,
    0x22,
    0x22,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0x70 [] */
    0x3e,
    0x28,
    0x28,
    0x28,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x71 [] */
    0x10,
    0x28,
    0x28,
    0x18,
    0x3e,
    0 ,
  6,	/* AsciiCode = 0x72 [] */
    0x3e,
    0x10,
    0x20,
    0x20,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x73 [] */
    0x12,
    0x2a,
    0x2a,
    0x2a,
    0x4,
    0 ,
  6,	/* AsciiCode = 0x74 [] */
    0x20,
    0xfc,
    0x22,
    0x2,
    0x4,
    0 ,
  6,	/* AsciiCode = 0x75 [] */
    0x3c,
    0x2,
    0x2,
    0x4,
    0x3e,
    0 ,
  6,	/* AsciiCode = 0x76 [] */
    0x38,
    0x4,
    0x2,
    0x4,
    0x38,
    0 ,
  6,	/* AsciiCode = 0x77 [] */
    0x3c,
    0x2,
    0xc,
    0x2,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0x78 [] */
    0x22,
    0x14,
    0x8,
    0x14,
    0x22,
    0 ,
  6,	/* AsciiCode = 0x79 [] */
    0x30,
    0xa,
    0xa,
    0xa,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0x7a [] */
    0x22,
    0x26,
    0x2a,
    0x32,
    0x22,
    0 ,
  6,	/* AsciiCode = 0x7b [] */
    0 ,
    0x10,
    0x6c,
    0x82,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x7c [] */
    0 ,
    0 ,
    0xfe,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x7d [] */
    0 ,
    0x82,
    0x6c,
    0x10,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x7e [] */
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0 ,
  4,	/* AsciiCode = 0x7f [] */
    0x14,
    0x8,
    0x14,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x80 [] */
    0x4,
    0xaa,
    0x6a,
    0x2a,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x81 [] */
    0x4,
    0x2a,
    0x6a,
    0xaa,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x82 [] */
    0x4,
    0x6a,
    0xaa,
    0x6a,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x83 [] */
    0x4,
    0xaa,
    0x2a,
    0xaa,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0x84 [] */
    0x30,
    0x4a,
    0x4e,
    0x48,
    0x10,
    0 ,
  6,	/* AsciiCode = 0x85 [] */
    0x1c,
    0xaa,
    0x6a,
    0x2a,
    0x18,
    0 ,
  6,	/* AsciiCode = 0x86 [] */
    0x1c,
    0x2a,
    0x6a,
    0xaa,
    0x18,
    0 ,
  6,	/* AsciiCode = 0x87 [] */
    0x1c,
    0x6a,
    0xaa,
    0x6a,
    0x18,
    0 ,
  6,	/* AsciiCode = 0x88 [] */
    0x1c,
    0xaa,
    0x2a,
    0xaa,
    0x18,
    0 ,
  6,	/* AsciiCode = 0x89 [] */
    0 ,
    0xa2,
    0x7e,
    0x2,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x8a [] */
    0 ,
    0x22,
    0x7e,
    0x82,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x8b [] */
    0 ,
    0x62,
    0xbe,
    0x42,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x8c [] */
    0 ,
    0xa2,
    0x3e,
    0x82,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x8d [] */
    0x1e,
    0x48,
    0x90,
    0x50,
    0x8e,
    0 ,
  6,	/* AsciiCode = 0x8e [] */
    0x1c,
    0xa2,
    0x62,
    0x22,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0x8f [] */
    0x1c,
    0x22,
    0x62,
    0xa2,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0x90 [] */
    0x1c,
    0x62,
    0xa2,
    0x62,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0x91 [] */
    0x1c,
    0xa2,
    0x22,
    0xa2,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0x92 [] */
    0x1c,
    0x22,
    0x3e,
    0x2a,
    0x3a,
    0 ,
  6,	/* AsciiCode = 0x93 [] */
    0x3c,
    0x82,
    0x42,
    0x4,
    0x3e,
    0 ,
  6,	/* AsciiCode = 0x94 [] */
    0x3c,
    0x2,
    0x42,
    0x84,
    0x3e,
    0 ,
  6,	/* AsciiCode = 0x95 [] */
    0x3c,
    0x42,
    0x82,
    0x44,
    0x3e,
    0 ,
  6,	/* AsciiCode = 0x96 [] */
    0x3c,
    0x82,
    0x2,
    0x84,
    0x3e,
    0 ,
  6,	/* AsciiCode = 0x97 [] */
    0x7e,
    0xa8,
    0xa8,
    0xa8,
    0x50,
    0 ,
  6,	/* AsciiCode = 0x98 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x99 [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x9a [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x9b [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x9c [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x9d [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x9e [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0x9f [] */
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xa0 [] */
    0xf8,
    0x88,
    0xf8,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xa1 [] */
    0xe,
    0xa,
    0xe,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xa2 [] */
    0 ,
    0 ,
    0xf0,
    0x80,
    0x80,
    0 ,
  6,	/* AsciiCode = 0xa3 [] */
    0x2,
    0x2,
    0x1e,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xa4 [] */
    0x8,
    0x4,
    0x2,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xa5 [] */
    0 ,
    0x18,
    0x18,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xa6 [] */
    0x50,
    0x50,
    0x52,
    0x54,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xa7 [] */
    0x20,
    0x22,
    0x2c,
    0x28,
    0x30,
    0 ,
  6,	/* AsciiCode = 0xa8 [] */
    0x4,
    0x8,
    0x1e,
    0x20,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xa9 [] */
    0x18,
    0x10,
    0x32,
    0x12,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0xaa [] */
    0x12,
    0x12,
    0x1e,
    0x12,
    0x12,
    0 ,
  6,	/* AsciiCode = 0xab [] */
    0x12,
    0x14,
    0x18,
    0x3e,
    0x10,
    0 ,
  6,	/* AsciiCode = 0xac [] */
    0x10,
    0x3e,
    0x10,
    0x14,
    0x18,
    0 ,
  6,	/* AsciiCode = 0xad [] */
    0x2,
    0x12,
    0x12,
    0x1e,
    0x2,
    0 ,
  6,	/* AsciiCode = 0xae [] */
    0x2a,
    0x2a,
    0x2a,
    0x3e,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xaf [] */
    0x18,
    0 ,
    0x1a,
    0x2,
    0x1c,
    0 ,
  6,	/* AsciiCode = 0xb0 [] */
    0x10,
    0x10,
    0x10,
    0x10,
    0x10,
    0 ,
  6,	/* AsciiCode = 0xb1 [] */
    0x80,
    0x82,
    0xbc,
    0x90,
    0xe0,
    0 ,
  6,	/* AsciiCode = 0xb2 [] */
    0x8,
    0x10,
    0x3e,
    0x40,
    0x80,
    0 ,
  6,	/* AsciiCode = 0xb3 [] */
    0x70,
    0x40,
    0xc2,
    0x44,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xb4 [] */
    0x42,
    0x42,
    0x7e,
    0x42,
    0x42,
    0 ,
  6,	/* AsciiCode = 0xb5 [] */
    0x44,
    0x48,
    0x50,
    0xfe,
    0x40,
    0 ,
  6,	/* AsciiCode = 0xb6 [] */
    0x42,
    0xfc,
    0x40,
    0x42,
    0x7c,
    0 ,
  6,	/* AsciiCode = 0xb7 [] */
    0x50,
    0x50,
    0xfe,
    0x50,
    0x50,
    0 ,
  6,	/* AsciiCode = 0xb8 [] */
    0x10,
    0x62,
    0x42,
    0x44,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xb9 [] */
    0x20,
    0xc0,
    0x42,
    0x7c,
    0x40,
    0 ,
  6,	/* AsciiCode = 0xba [] */
    0x42,
    0x42,
    0x42,
    0x42,
    0x7e,
    0 ,
  6,	/* AsciiCode = 0xbb [] */
    0x40,
    0xf2,
    0x44,
    0xf8,
    0x40,
    0 ,
  6,	/* AsciiCode = 0xbc [] */
    0x52,
    0x52,
    0x2,
    0x4,
    0x38,
    0 ,
  6,	/* AsciiCode = 0xbd [] */
    0x42,
    0x44,
    0x48,
    0x54,
    0x62,
    0 ,
  6,	/* AsciiCode = 0xbe [] */
    0x40,
    0xfc,
    0x42,
    0x52,
    0x62,
    0 ,
  6,	/* AsciiCode = 0xbf [] */
    0x60,
    0x12,
    0x2,
    0x4,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xc0 [] */
    0x10,
    0x62,
    0x52,
    0x4c,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xc1 [] */
    0x50,
    0x52,
    0x7c,
    0x90,
    0x10,
    0 ,
  6,	/* AsciiCode = 0xc2 [] */
    0x70,
    0 ,
    0x72,
    0x4,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xc3 [] */
    0x20,
    0xa2,
    0xbc,
    0xa0,
    0x20,
    0 ,
  6,	/* AsciiCode = 0xc4 [] */
    0 ,
    0xfe,
    0x10,
    0x8,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xc5 [] */
    0x22,
    0x24,
    0xf8,
    0x20,
    0x20,
    0 ,
  6,	/* AsciiCode = 0xc6 [] */
    0x2,
    0x42,
    0x42,
    0x42,
    0x2,
    0 ,
  6,	/* AsciiCode = 0xc7 [] */
    0x42,
    0x54,
    0x48,
    0x54,
    0x60,
    0 ,
  6,	/* AsciiCode = 0xc8 [] */
    0x44,
    0x48,
    0xde,
    0x68,
    0x44,
    0 ,
  6,	/* AsciiCode = 0xc9 [] */
    0 ,
    0x2,
    0x4,
    0xf8,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xca [] */
    0x1e,
    0 ,
    0x40,
    0x20,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0xcb [] */
    0xfc,
    0x22,
    0x22,
    0x22,
    0x22,
    0 ,
  6,	/* AsciiCode = 0xcc [] */
    0x40,
    0x40,
    0x42,
    0x44,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xcd [] */
    0x20,
    0x40,
    0x20,
    0x10,
    0xc,
    0 ,
  6,	/* AsciiCode = 0xce [] */
    0x4c,
    0x40,
    0xfe,
    0x40,
    0x4c,
    0 ,
  6,	/* AsciiCode = 0xcf [] */
    0x40,
    0x48,
    0x44,
    0x4a,
    0x70,
    0 ,
  6,	/* AsciiCode = 0xd0 [] */
    0 ,
    0x54,
    0x54,
    0x54,
    0x2,
    0 ,
  6,	/* AsciiCode = 0xd1 [] */
    0x1c,
    0x24,
    0x44,
    0x4,
    0xe,
    0 ,
  6,	/* AsciiCode = 0xd2 [] */
    0x2,
    0x14,
    0x8,
    0x14,
    0x60,
    0 ,
  6,	/* AsciiCode = 0xd3 [] */
    0x50,
    0x7c,
    0x52,
    0x52,
    0x52,
    0 ,
  6,	/* AsciiCode = 0xd4 [] */
    0x20,
    0xfe,
    0x20,
    0x28,
    0x30,
    0 ,
  6,	/* AsciiCode = 0xd5 [] */
    0x2,
    0x42,
    0x42,
    0x7e,
    0x2,
    0 ,
  6,	/* AsciiCode = 0xd6 [] */
    0x52,
    0x52,
    0x52,
    0x52,
    0x7e,
    0 ,
  6,	/* AsciiCode = 0xd7 [] */
    0x20,
    0xa0,
    0xa2,
    0xa4,
    0x38,
    0 ,
  6,	/* AsciiCode = 0xd8 [] */
    0x70,
    0 ,
    0x2,
    0x4,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xd9 [] */
    0x3e,
    0 ,
    0x7e,
    0x2,
    0xc,
    0 ,
  6,	/* AsciiCode = 0xda [] */
    0x7e,
    0x2,
    0x4,
    0x8,
    0x10,
    0 ,
  6,	/* AsciiCode = 0xdb [] */
    0x7e,
    0x42,
    0x42,
    0x42,
    0x7e,
    0 ,
  6,	/* AsciiCode = 0xdc [] */
    0x60,
    0x40,
    0x42,
    0x44,
    0x78,
    0 ,
  6,	/* AsciiCode = 0xdd [] */
    0x42,
    0x42,
    0x2,
    0x4,
    0x18,
    0 ,
  6,	/* AsciiCode = 0xde [] */
    0x40,
    0x20,
    0x80,
    0x40,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xdf [] */
    0xe0,
    0xa0,
    0xe0,
    0 ,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xe0 [] */
    0x1e,
    0xa8,
    0x68,
    0x28,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0xe1 [] */
    0x1e,
    0x28,
    0x68,
    0xa8,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0xe2 [] */
    0x1e,
    0x68,
    0xa8,
    0x68,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0xe3 [] */
    0x1e,
    0xa8,
    0x28,
    0xa8,
    0x1e,
    0 ,
  6,	/* AsciiCode = 0xe4 [] */
    0x70,
    0x8a,
    0x8e,
    0x88,
    0x50,
    0 ,
  6,	/* AsciiCode = 0xe5 [] */
    0x3e,
    0xaa,
    0x6a,
    0x2a,
    0x22,
    0 ,
  6,	/* AsciiCode = 0xe6 [] */
    0x3e,
    0x2a,
    0x6a,
    0xaa,
    0x22,
    0 ,
  6,	/* AsciiCode = 0xe7 [] */
    0x3e,
    0x6a,
    0xaa,
    0x6a,
    0x22,
    0 ,
  6,	/* AsciiCode = 0xe8 [] */
    0x3e,
    0xaa,
    0x2a,
    0xaa,
    0x22,
    0 ,
  6,	/* AsciiCode = 0xe9 [] */
    0 ,
    0xa2,
    0x7e,
    0x22,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xea [] */
    0 ,
    0x22,
    0x7e,
    0xa2,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xeb [] */
    0 ,
    0x62,
    0xbe,
    0x62,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xec [] */
    0 ,
    0xa2,
    0x3e,
    0xa2,
    0 ,
    0 ,
  6,	/* AsciiCode = 0xed [] */
    0x3e,
    0x50,
    0x88,
    0x44,
    0xbe,
    0 ,
  6,	/* AsciiCode = 0xee [] */
    0x3c,
    0xc2,
    0x42,
    0x42,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0xef [] */
    0x3c,
    0x42,
    0x42,
    0xc2,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0xf0 [] */
    0x3c,
    0x42,
    0xc2,
    0x42,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0xf1 [] */
    0x3c,
    0xc2,
    0x42,
    0xc2,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0xf2 [] */
    0x7c,
    0x82,
    0xfe,
    0x92,
    0x92,
    0 ,
  6,	/* AsciiCode = 0xf3 [] */
    0x3c,
    0x82,
    0x42,
    0x2,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0xf4 [] */
    0x3c,
    0x2,
    0x42,
    0x82,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0xf5 [] */
    0x3c,
    0x42,
    0x82,
    0x42,
    0x3c,
    0 ,
  6,	/* AsciiCode = 0xf6 [] */
    0x3c,
    0x82,
    0x2,
    0x82,
    0x3c,
    0 ,
  3,	/* AsciiCode = 0xf7 [] */
    0x40,
    0xf8,
    0 ,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xf8 [] */
    0xb8,
    0xa8,
    0xe8,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xf9 [] */
    0xa8,
    0xa8,
    0xf8,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xfa [] */
    0xe0,
    0x20,
    0xf8,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xfb [] */
    0xe8,
    0xa8,
    0xb8,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xfc [] */
    0xf8,
    0xa8,
    0xb8,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xfd [] */
    0x80,
    0x98,
    0xe0,
    0 ,
    0 ,
    0 ,
  4,	/* AsciiCode = 0xfe [] */
    0xf8,
    0xa8,
    0xf8,
    0 ,
    0 ,
    0 ,
//  4,	/* AsciiCode = 0xff [] */
//    0xe8,
//    0xa8,
//    0xf8,
//    0 ,
//    0 ,
//    0 ,
}
};
