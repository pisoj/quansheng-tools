#ifndef _LIBQUANSHANG_CHANNELS_CHANN_H
#define _LIBQUANSHANG_CHANNELS_CHANN_H

#include <stdint.h>

#define _Chan_CHANNEL_NAME_MAX_CHARACTERS 10 // More cannot fit on radio screen

#define _ChanCode_TYPE_NONE 0x000
#define _ChanCode_TYPE_CTCSS 0x100
#define _ChanCode_TYPE_DCS 0x200
#define _ChanCode_TYPE_DCS_INVERTED 0x300

typedef enum ChanCode {
    ChanCode_NONE = _ChanCode_TYPE_NONE,

    ChanCode_CTCSS_67 = _ChanCode_TYPE_CTCSS | 0x00,
    ChanCode_CTCSS_69_3 = _ChanCode_TYPE_CTCSS | 0x01,
    ChanCode_CTCSS_71_9 = _ChanCode_TYPE_CTCSS | 0x02,
    ChanCode_CTCSS_74_4 = _ChanCode_TYPE_CTCSS | 0x03,
    ChanCode_CTCSS_77 = _ChanCode_TYPE_CTCSS | 0x04,
    ChanCode_CTCSS_79_7 = _ChanCode_TYPE_CTCSS | 0x05,
    ChanCode_CTCSS_82_5 = _ChanCode_TYPE_CTCSS | 0x06,
    ChanCode_CTCSS_85_4 = _ChanCode_TYPE_CTCSS | 0x07,
    ChanCode_CTCSS_88_5 = _ChanCode_TYPE_CTCSS | 0x08,
    ChanCode_CTCSS_91_5 = _ChanCode_TYPE_CTCSS | 0x09,
    ChanCode_CTCSS_94_8 = _ChanCode_TYPE_CTCSS | 0x0A,
    ChanCode_CTCSS_97_4 = _ChanCode_TYPE_CTCSS | 0x0B,
    ChanCode_CTCSS_100 = _ChanCode_TYPE_CTCSS | 0x0C,
    ChanCode_CTCSS_103_5 = _ChanCode_TYPE_CTCSS | 0x0D,
    ChanCode_CTCSS_107_2 = _ChanCode_TYPE_CTCSS | 0x0E,
    ChanCode_CTCSS_110_9 = _ChanCode_TYPE_CTCSS | 0x0F,
    ChanCode_CTCSS_114_8 = _ChanCode_TYPE_CTCSS | 0x10,
    ChanCode_CTCSS_118_8 = _ChanCode_TYPE_CTCSS | 0x11,
    ChanCode_CTCSS_123 = _ChanCode_TYPE_CTCSS | 0x12,
    ChanCode_CTCSS_127_3 = _ChanCode_TYPE_CTCSS | 0x13,
    ChanCode_CTCSS_131_8 = _ChanCode_TYPE_CTCSS | 0x14,
    ChanCode_CTCSS_136_5 = _ChanCode_TYPE_CTCSS | 0x15,
    ChanCode_CTCSS_141_3 = _ChanCode_TYPE_CTCSS | 0x16,
    ChanCode_CTCSS_146_2 = _ChanCode_TYPE_CTCSS | 0x17,
    ChanCode_CTCSS_151_4 = _ChanCode_TYPE_CTCSS | 0x18,
    ChanCode_CTCSS_156_7 = _ChanCode_TYPE_CTCSS | 0x19,
    ChanCode_CTCSS_159_8 = _ChanCode_TYPE_CTCSS | 0x1A,
    ChanCode_CTCSS_162_2 = _ChanCode_TYPE_CTCSS | 0x1B,
    ChanCode_CTCSS_165_5 = _ChanCode_TYPE_CTCSS | 0x1C,
    ChanCode_CTCSS_167_9 = _ChanCode_TYPE_CTCSS | 0x1D,
    ChanCode_CTCSS_171_3 = _ChanCode_TYPE_CTCSS | 0x1E,
    ChanCode_CTCSS_173_8 = _ChanCode_TYPE_CTCSS | 0x1F,
    ChanCode_CTCSS_177_3 = _ChanCode_TYPE_CTCSS | 0x20,
    ChanCode_CTCSS_179_9 = _ChanCode_TYPE_CTCSS | 0x21,
    ChanCode_CTCSS_183_5 = _ChanCode_TYPE_CTCSS | 0x22,
    ChanCode_CTCSS_186_2 = _ChanCode_TYPE_CTCSS | 0x23,
    ChanCode_CTCSS_189_9 = _ChanCode_TYPE_CTCSS | 0x24,
    ChanCode_CTCSS_192_8 = _ChanCode_TYPE_CTCSS | 0x25,
    ChanCode_CTCSS_196_6 = _ChanCode_TYPE_CTCSS | 0x26,
    ChanCode_CTCSS_199_5 = _ChanCode_TYPE_CTCSS | 0x27,
    ChanCode_CTCSS_203_5 = _ChanCode_TYPE_CTCSS | 0x28,
    ChanCode_CTCSS_206_5 = _ChanCode_TYPE_CTCSS | 0x29,
    ChanCode_CTCSS_210_7 = _ChanCode_TYPE_CTCSS | 0x2A,
    ChanCode_CTCSS_218_1 = _ChanCode_TYPE_CTCSS | 0x2B,
    ChanCode_CTCSS_225_7 = _ChanCode_TYPE_CTCSS | 0x2C,
    ChanCode_CTCSS_229_1 = _ChanCode_TYPE_CTCSS | 0x2D,
    ChanCode_CTCSS_233_6 = _ChanCode_TYPE_CTCSS | 0x2E,
    ChanCode_CTCSS_241_8 = _ChanCode_TYPE_CTCSS | 0x2F,
    ChanCode_CTCSS_250_3 = _ChanCode_TYPE_CTCSS | 0x30,
    ChanCode_CTCSS_254_1 = _ChanCode_TYPE_CTCSS | 0x31,

    ChanCode_DCS_23 = _ChanCode_TYPE_DCS | 0x00,
    ChanCode_DCS_25 = _ChanCode_TYPE_DCS | 0x01,
    ChanCode_DCS_26 = _ChanCode_TYPE_DCS | 0x02,
    ChanCode_DCS_31 = _ChanCode_TYPE_DCS | 0x03,
    ChanCode_DCS_32 = _ChanCode_TYPE_DCS | 0x04,
    ChanCode_DCS_36 = _ChanCode_TYPE_DCS | 0x05,
    ChanCode_DCS_43 = _ChanCode_TYPE_DCS | 0x06,
    ChanCode_DCS_47 = _ChanCode_TYPE_DCS | 0x07,
    ChanCode_DCS_51 = _ChanCode_TYPE_DCS | 0x08,
    ChanCode_DCS_53 = _ChanCode_TYPE_DCS | 0x09,
    ChanCode_DCS_54 = _ChanCode_TYPE_DCS | 0x0A,
    ChanCode_DCS_65 = _ChanCode_TYPE_DCS | 0x0B,
    ChanCode_DCS_71 = _ChanCode_TYPE_DCS | 0x0C,
    ChanCode_DCS_72 = _ChanCode_TYPE_DCS | 0x0D,
    ChanCode_DCS_73 = _ChanCode_TYPE_DCS | 0x0E,
    ChanCode_DCS_74 = _ChanCode_TYPE_DCS | 0x0F,
    ChanCode_DCS_114 = _ChanCode_TYPE_DCS | 0x10,
    ChanCode_DCS_115 = _ChanCode_TYPE_DCS | 0x11,
    ChanCode_DCS_116 = _ChanCode_TYPE_DCS | 0x12,
    ChanCode_DCS_122 = _ChanCode_TYPE_DCS | 0x13,
    ChanCode_DCS_125 = _ChanCode_TYPE_DCS | 0x14,
    ChanCode_DCS_131 = _ChanCode_TYPE_DCS | 0x15,
    ChanCode_DCS_132 = _ChanCode_TYPE_DCS | 0x16,
    ChanCode_DCS_134 = _ChanCode_TYPE_DCS | 0x17,
    ChanCode_DCS_143 = _ChanCode_TYPE_DCS | 0x18,
    ChanCode_DCS_145 = _ChanCode_TYPE_DCS | 0x19,
    ChanCode_DCS_152 = _ChanCode_TYPE_DCS | 0x1A,
    ChanCode_DCS_155 = _ChanCode_TYPE_DCS | 0x1B,
    ChanCode_DCS_156 = _ChanCode_TYPE_DCS | 0x1C,
    ChanCode_DCS_162 = _ChanCode_TYPE_DCS | 0x1D,
    ChanCode_DCS_165 = _ChanCode_TYPE_DCS | 0x1E,
    ChanCode_DCS_172 = _ChanCode_TYPE_DCS | 0x1F,
    ChanCode_DCS_174 = _ChanCode_TYPE_DCS | 0x20,
    ChanCode_DCS_205 = _ChanCode_TYPE_DCS | 0x21,
    ChanCode_DCS_212 = _ChanCode_TYPE_DCS | 0x22,
    ChanCode_DCS_223 = _ChanCode_TYPE_DCS | 0x23,
    ChanCode_DCS_225 = _ChanCode_TYPE_DCS | 0x24,
    ChanCode_DCS_226 = _ChanCode_TYPE_DCS | 0x25,
    ChanCode_DCS_243 = _ChanCode_TYPE_DCS | 0x26,
    ChanCode_DCS_244 = _ChanCode_TYPE_DCS | 0x27,
    ChanCode_DCS_245 = _ChanCode_TYPE_DCS | 0x28,
    ChanCode_DCS_246 = _ChanCode_TYPE_DCS | 0x29,
    ChanCode_DCS_251 = _ChanCode_TYPE_DCS | 0x2A,
    ChanCode_DCS_252 = _ChanCode_TYPE_DCS | 0x2B,
    ChanCode_DCS_255 = _ChanCode_TYPE_DCS | 0x2C,
    ChanCode_DCS_261 = _ChanCode_TYPE_DCS | 0x2D,
    ChanCode_DCS_263 = _ChanCode_TYPE_DCS | 0x2E,
    ChanCode_DCS_265 = _ChanCode_TYPE_DCS | 0x2F,
    ChanCode_DCS_266 = _ChanCode_TYPE_DCS | 0x30,
    ChanCode_DCS_271 = _ChanCode_TYPE_DCS | 0x31,
    ChanCode_DCS_274 = _ChanCode_TYPE_DCS | 0x32,
    ChanCode_DCS_306 = _ChanCode_TYPE_DCS | 0x33,
    ChanCode_DCS_311 = _ChanCode_TYPE_DCS | 0x34,
    ChanCode_DCS_315 = _ChanCode_TYPE_DCS | 0x35,
    ChanCode_DCS_325 = _ChanCode_TYPE_DCS | 0x36,
    ChanCode_DCS_331 = _ChanCode_TYPE_DCS | 0x37,
    ChanCode_DCS_332 = _ChanCode_TYPE_DCS | 0x38,
    ChanCode_DCS_343 = _ChanCode_TYPE_DCS | 0x39,
    ChanCode_DCS_346 = _ChanCode_TYPE_DCS | 0x3A,
    ChanCode_DCS_351 = _ChanCode_TYPE_DCS | 0x3B,
    ChanCode_DCS_356 = _ChanCode_TYPE_DCS | 0x3C,
    ChanCode_DCS_364 = _ChanCode_TYPE_DCS | 0x3D,
    ChanCode_DCS_365 = _ChanCode_TYPE_DCS | 0x3E,
    ChanCode_DCS_371 = _ChanCode_TYPE_DCS | 0x3F,
    ChanCode_DCS_411 = _ChanCode_TYPE_DCS | 0x40,
    ChanCode_DCS_412 = _ChanCode_TYPE_DCS | 0x41,
    ChanCode_DCS_413 = _ChanCode_TYPE_DCS | 0x42,
    ChanCode_DCS_423 = _ChanCode_TYPE_DCS | 0x43,
    ChanCode_DCS_431 = _ChanCode_TYPE_DCS | 0x44,
    ChanCode_DCS_432 = _ChanCode_TYPE_DCS | 0x45,
    ChanCode_DCS_445 = _ChanCode_TYPE_DCS | 0x46,
    ChanCode_DCS_446 = _ChanCode_TYPE_DCS | 0x47,
    ChanCode_DCS_452 = _ChanCode_TYPE_DCS | 0x48,
    ChanCode_DCS_454 = _ChanCode_TYPE_DCS | 0x49,
    ChanCode_DCS_455 = _ChanCode_TYPE_DCS | 0x4A,
    ChanCode_DCS_462 = _ChanCode_TYPE_DCS | 0x4B,
    ChanCode_DCS_464 = _ChanCode_TYPE_DCS | 0x4C,
    ChanCode_DCS_465 = _ChanCode_TYPE_DCS | 0x4D,
    ChanCode_DCS_466 = _ChanCode_TYPE_DCS | 0x4E,
    ChanCode_DCS_503 = _ChanCode_TYPE_DCS | 0x4F,
    ChanCode_DCS_506 = _ChanCode_TYPE_DCS | 0x50,
    ChanCode_DCS_516 = _ChanCode_TYPE_DCS | 0x51,
    ChanCode_DCS_523 = _ChanCode_TYPE_DCS | 0x52,
    ChanCode_DCS_526 = _ChanCode_TYPE_DCS | 0x53,
    ChanCode_DCS_532 = _ChanCode_TYPE_DCS | 0x54,
    ChanCode_DCS_546 = _ChanCode_TYPE_DCS | 0x55,
    ChanCode_DCS_565 = _ChanCode_TYPE_DCS | 0x56,
    ChanCode_DCS_606 = _ChanCode_TYPE_DCS | 0x57,
    ChanCode_DCS_612 = _ChanCode_TYPE_DCS | 0x58,
    ChanCode_DCS_624 = _ChanCode_TYPE_DCS | 0x59,
    ChanCode_DCS_627 = _ChanCode_TYPE_DCS | 0x5A,
    ChanCode_DCS_631 = _ChanCode_TYPE_DCS | 0x5B,
    ChanCode_DCS_632 = _ChanCode_TYPE_DCS | 0x5C,
    ChanCode_DCS_654 = _ChanCode_TYPE_DCS | 0x5D,
    ChanCode_DCS_662 = _ChanCode_TYPE_DCS | 0x5E,
    ChanCode_DCS_664 = _ChanCode_TYPE_DCS | 0x5F,
    ChanCode_DCS_703 = _ChanCode_TYPE_DCS | 0x60,
    ChanCode_DCS_712 = _ChanCode_TYPE_DCS | 0x61,
    ChanCode_DCS_723 = _ChanCode_TYPE_DCS | 0x62,
    ChanCode_DCS_731 = _ChanCode_TYPE_DCS | 0x63,
    ChanCode_DCS_732 = _ChanCode_TYPE_DCS | 0x64,
    ChanCode_DCS_734 = _ChanCode_TYPE_DCS | 0x65,
    ChanCode_DCS_743 = _ChanCode_TYPE_DCS | 0x66,
    ChanCode_DCS_754 = _ChanCode_TYPE_DCS | 0x67,

    ChanCode_DCS_INVERTED_23 = _ChanCode_TYPE_DCS_INVERTED | 0x00,
    ChanCode_DCS_INVERTED_25 = _ChanCode_TYPE_DCS_INVERTED | 0x01,
    ChanCode_DCS_INVERTED_26 = _ChanCode_TYPE_DCS_INVERTED | 0x02,
    ChanCode_DCS_INVERTED_31 = _ChanCode_TYPE_DCS_INVERTED | 0x03,
    ChanCode_DCS_INVERTED_32 = _ChanCode_TYPE_DCS_INVERTED | 0x04,
    ChanCode_DCS_INVERTED_36 = _ChanCode_TYPE_DCS_INVERTED | 0x05,
    ChanCode_DCS_INVERTED_43 = _ChanCode_TYPE_DCS_INVERTED | 0x06,
    ChanCode_DCS_INVERTED_47 = _ChanCode_TYPE_DCS_INVERTED | 0x07,
    ChanCode_DCS_INVERTED_51 = _ChanCode_TYPE_DCS_INVERTED | 0x08,
    ChanCode_DCS_INVERTED_53 = _ChanCode_TYPE_DCS_INVERTED | 0x09,
    ChanCode_DCS_INVERTED_54 = _ChanCode_TYPE_DCS_INVERTED | 0x0A,
    ChanCode_DCS_INVERTED_65 = _ChanCode_TYPE_DCS_INVERTED | 0x0B,
    ChanCode_DCS_INVERTED_71 = _ChanCode_TYPE_DCS_INVERTED | 0x0C,
    ChanCode_DCS_INVERTED_72 = _ChanCode_TYPE_DCS_INVERTED | 0x0D,
    ChanCode_DCS_INVERTED_73 = _ChanCode_TYPE_DCS_INVERTED | 0x0E,
    ChanCode_DCS_INVERTED_74 = _ChanCode_TYPE_DCS_INVERTED | 0x0F,
    ChanCode_DCS_INVERTED_114 = _ChanCode_TYPE_DCS_INVERTED | 0x10,
    ChanCode_DCS_INVERTED_115 = _ChanCode_TYPE_DCS_INVERTED | 0x11,
    ChanCode_DCS_INVERTED_116 = _ChanCode_TYPE_DCS_INVERTED | 0x12,
    ChanCode_DCS_INVERTED_122 = _ChanCode_TYPE_DCS_INVERTED | 0x13,
    ChanCode_DCS_INVERTED_125 = _ChanCode_TYPE_DCS_INVERTED | 0x14,
    ChanCode_DCS_INVERTED_131 = _ChanCode_TYPE_DCS_INVERTED | 0x15,
    ChanCode_DCS_INVERTED_132 = _ChanCode_TYPE_DCS_INVERTED | 0x16,
    ChanCode_DCS_INVERTED_134 = _ChanCode_TYPE_DCS_INVERTED | 0x17,
    ChanCode_DCS_INVERTED_143 = _ChanCode_TYPE_DCS_INVERTED | 0x18,
    ChanCode_DCS_INVERTED_145 = _ChanCode_TYPE_DCS_INVERTED | 0x19,
    ChanCode_DCS_INVERTED_152 = _ChanCode_TYPE_DCS_INVERTED | 0x1A,
    ChanCode_DCS_INVERTED_155 = _ChanCode_TYPE_DCS_INVERTED | 0x1B,
    ChanCode_DCS_INVERTED_156 = _ChanCode_TYPE_DCS_INVERTED | 0x1C,
    ChanCode_DCS_INVERTED_162 = _ChanCode_TYPE_DCS_INVERTED | 0x1D,
    ChanCode_DCS_INVERTED_165 = _ChanCode_TYPE_DCS_INVERTED | 0x1E,
    ChanCode_DCS_INVERTED_172 = _ChanCode_TYPE_DCS_INVERTED | 0x1F,
    ChanCode_DCS_INVERTED_174 = _ChanCode_TYPE_DCS_INVERTED | 0x20,
    ChanCode_DCS_INVERTED_205 = _ChanCode_TYPE_DCS_INVERTED | 0x21,
    ChanCode_DCS_INVERTED_212 = _ChanCode_TYPE_DCS_INVERTED | 0x22,
    ChanCode_DCS_INVERTED_223 = _ChanCode_TYPE_DCS_INVERTED | 0x23,
    ChanCode_DCS_INVERTED_225 = _ChanCode_TYPE_DCS_INVERTED | 0x24,
    ChanCode_DCS_INVERTED_226 = _ChanCode_TYPE_DCS_INVERTED | 0x25,
    ChanCode_DCS_INVERTED_243 = _ChanCode_TYPE_DCS_INVERTED | 0x26,
    ChanCode_DCS_INVERTED_244 = _ChanCode_TYPE_DCS_INVERTED | 0x27,
    ChanCode_DCS_INVERTED_245 = _ChanCode_TYPE_DCS_INVERTED | 0x28,
    ChanCode_DCS_INVERTED_246 = _ChanCode_TYPE_DCS_INVERTED | 0x29,
    ChanCode_DCS_INVERTED_251 = _ChanCode_TYPE_DCS_INVERTED | 0x2A,
    ChanCode_DCS_INVERTED_252 = _ChanCode_TYPE_DCS_INVERTED | 0x2B,
    ChanCode_DCS_INVERTED_255 = _ChanCode_TYPE_DCS_INVERTED | 0x2C,
    ChanCode_DCS_INVERTED_261 = _ChanCode_TYPE_DCS_INVERTED | 0x2D,
    ChanCode_DCS_INVERTED_263 = _ChanCode_TYPE_DCS_INVERTED | 0x2E,
    ChanCode_DCS_INVERTED_265 = _ChanCode_TYPE_DCS_INVERTED | 0x2F,
    ChanCode_DCS_INVERTED_266 = _ChanCode_TYPE_DCS_INVERTED | 0x30,
    ChanCode_DCS_INVERTED_271 = _ChanCode_TYPE_DCS_INVERTED | 0x31,
    ChanCode_DCS_INVERTED_274 = _ChanCode_TYPE_DCS_INVERTED | 0x32,
    ChanCode_DCS_INVERTED_306 = _ChanCode_TYPE_DCS_INVERTED | 0x33,
    ChanCode_DCS_INVERTED_311 = _ChanCode_TYPE_DCS_INVERTED | 0x34,
    ChanCode_DCS_INVERTED_315 = _ChanCode_TYPE_DCS_INVERTED | 0x35,
    ChanCode_DCS_INVERTED_325 = _ChanCode_TYPE_DCS_INVERTED | 0x36,
    ChanCode_DCS_INVERTED_331 = _ChanCode_TYPE_DCS_INVERTED | 0x37,
    ChanCode_DCS_INVERTED_332 = _ChanCode_TYPE_DCS_INVERTED | 0x38,
    ChanCode_DCS_INVERTED_343 = _ChanCode_TYPE_DCS_INVERTED | 0x39,
    ChanCode_DCS_INVERTED_346 = _ChanCode_TYPE_DCS_INVERTED | 0x3A,
    ChanCode_DCS_INVERTED_351 = _ChanCode_TYPE_DCS_INVERTED | 0x3B,
    ChanCode_DCS_INVERTED_356 = _ChanCode_TYPE_DCS_INVERTED | 0x3C,
    ChanCode_DCS_INVERTED_364 = _ChanCode_TYPE_DCS_INVERTED | 0x3D,
    ChanCode_DCS_INVERTED_365 = _ChanCode_TYPE_DCS_INVERTED | 0x3E,
    ChanCode_DCS_INVERTED_371 = _ChanCode_TYPE_DCS_INVERTED | 0x3F,
    ChanCode_DCS_INVERTED_411 = _ChanCode_TYPE_DCS_INVERTED | 0x40,
    ChanCode_DCS_INVERTED_412 = _ChanCode_TYPE_DCS_INVERTED | 0x41,
    ChanCode_DCS_INVERTED_413 = _ChanCode_TYPE_DCS_INVERTED | 0x42,
    ChanCode_DCS_INVERTED_423 = _ChanCode_TYPE_DCS_INVERTED | 0x43,
    ChanCode_DCS_INVERTED_431 = _ChanCode_TYPE_DCS_INVERTED | 0x44,
    ChanCode_DCS_INVERTED_432 = _ChanCode_TYPE_DCS_INVERTED | 0x45,
    ChanCode_DCS_INVERTED_445 = _ChanCode_TYPE_DCS_INVERTED | 0x46,
    ChanCode_DCS_INVERTED_446 = _ChanCode_TYPE_DCS_INVERTED | 0x47,
    ChanCode_DCS_INVERTED_452 = _ChanCode_TYPE_DCS_INVERTED | 0x48,
    ChanCode_DCS_INVERTED_454 = _ChanCode_TYPE_DCS_INVERTED | 0x49,
    ChanCode_DCS_INVERTED_455 = _ChanCode_TYPE_DCS_INVERTED | 0x4A,
    ChanCode_DCS_INVERTED_462 = _ChanCode_TYPE_DCS_INVERTED | 0x4B,
    ChanCode_DCS_INVERTED_464 = _ChanCode_TYPE_DCS_INVERTED | 0x4C,
    ChanCode_DCS_INVERTED_465 = _ChanCode_TYPE_DCS_INVERTED | 0x4D,
    ChanCode_DCS_INVERTED_466 = _ChanCode_TYPE_DCS_INVERTED | 0x4E,
    ChanCode_DCS_INVERTED_503 = _ChanCode_TYPE_DCS_INVERTED | 0x4F,
    ChanCode_DCS_INVERTED_506 = _ChanCode_TYPE_DCS_INVERTED | 0x50,
    ChanCode_DCS_INVERTED_516 = _ChanCode_TYPE_DCS_INVERTED | 0x51,
    ChanCode_DCS_INVERTED_523 = _ChanCode_TYPE_DCS_INVERTED | 0x52,
    ChanCode_DCS_INVERTED_526 = _ChanCode_TYPE_DCS_INVERTED | 0x53,
    ChanCode_DCS_INVERTED_532 = _ChanCode_TYPE_DCS_INVERTED | 0x54,
    ChanCode_DCS_INVERTED_546 = _ChanCode_TYPE_DCS_INVERTED | 0x55,
    ChanCode_DCS_INVERTED_565 = _ChanCode_TYPE_DCS_INVERTED | 0x56,
    ChanCode_DCS_INVERTED_606 = _ChanCode_TYPE_DCS_INVERTED | 0x57,
    ChanCode_DCS_INVERTED_612 = _ChanCode_TYPE_DCS_INVERTED | 0x58,
    ChanCode_DCS_INVERTED_624 = _ChanCode_TYPE_DCS_INVERTED | 0x59,
    ChanCode_DCS_INVERTED_627 = _ChanCode_TYPE_DCS_INVERTED | 0x5A,
    ChanCode_DCS_INVERTED_631 = _ChanCode_TYPE_DCS_INVERTED | 0x5B,
    ChanCode_DCS_INVERTED_632 = _ChanCode_TYPE_DCS_INVERTED | 0x5C,
    ChanCode_DCS_INVERTED_654 = _ChanCode_TYPE_DCS_INVERTED | 0x5D,
    ChanCode_DCS_INVERTED_662 = _ChanCode_TYPE_DCS_INVERTED | 0x5E,
    ChanCode_DCS_INVERTED_664 = _ChanCode_TYPE_DCS_INVERTED | 0x5F,
    ChanCode_DCS_INVERTED_703 = _ChanCode_TYPE_DCS_INVERTED | 0x60,
    ChanCode_DCS_INVERTED_712 = _ChanCode_TYPE_DCS_INVERTED | 0x61,
    ChanCode_DCS_INVERTED_723 = _ChanCode_TYPE_DCS_INVERTED | 0x62,
    ChanCode_DCS_INVERTED_731 = _ChanCode_TYPE_DCS_INVERTED | 0x63,
    ChanCode_DCS_INVERTED_732 = _ChanCode_TYPE_DCS_INVERTED | 0x64,
    ChanCode_DCS_INVERTED_734 = _ChanCode_TYPE_DCS_INVERTED | 0x65,
    ChanCode_DCS_INVERTED_743 = _ChanCode_TYPE_DCS_INVERTED | 0x66,
    ChanCode_DCS_INVERTED_754 = _ChanCode_TYPE_DCS_INVERTED | 0x67,
} ChanCode;
typedef enum ChanModulation {
    ChanModulation_FM = 0x0,
    ChanModulation_AM = 0x1,
    ChanModulation_USB = 0x2,
} ChanModulation;
typedef enum ChanOffsetDirection {
    ChanOffsetDirection_NONE = 0x0,
    ChanOffsetDirection_PLUS = 0x1,
    ChanOffsetDirection_MINUS = 0x2,
} ChanOffsetDirection;
typedef enum ChanOutputPower {
    ChanOutputPower_LOW = 0x0,
    ChanOutputPower_MEDIUM = 0x1,
    ChanOutputPower_HIGH = 0x2,
} ChanOutputPower;
typedef enum ChanBandwidth {
    ChanBandwidth_WIDE = 0x0,
    ChanBandwidth_NARROW = 0x1,
} ChanBandwidth;
typedef enum ChanPttIdMode {
    ChanPttId_NONE = 0b000,
    ChanPttId_BEGIN = 0b001,
    ChanPttId_END = 0b010,
    ChanPttId_BEGIN_END = 0b011,
    ChanPttId_APOLLO_QUINDAR = 0b100,
} ChanPttIdMode;
typedef enum ChanFrequencyStep {
    ChanFrequencyStep_0_01 = 0x07,
    ChanFrequencyStep_0_05 = 0x08,
    ChanFrequencyStep_0_1 = 0x09,
    ChanFrequencyStep_0_25 = 0x0A,
    ChanFrequencyStep_0_5 = 0x0B,
    ChanFrequencyStep_1 = 0x0C,
    ChanFrequencyStep_1_25 = 0x0D,
    ChanFrequencyStep_2_5 = 0x00,
    ChanFrequencyStep_5 = 0x01,
    ChanFrequencyStep_6_25 = 0x02,
    ChanFrequencyStep_8_33 = 0x06,
    ChanFrequencyStep_10 = 0x03,
    ChanFrequencyStep_12_5 = 0x04,
    ChanFrequencyStep_15 = 0x0E,
    ChanFrequencyStep_25 = 0x05,
    ChanFrequencyStep_30 = 0x0F,
    ChanFrequencyStep_50 = 0x10,
    ChanFrequencyStep_100 = 0x11,
    ChanFrequencyStep_125 = 0x12,
    ChanFrequencyStep_250 = 0x13,
    ChanFrequencyStep_500 = 0x14,
} ChanFrequencyStep;
typedef enum ChanScrambleMode {
    ChanScramble_NONE = 0x00,
    ChanScramble_2600 = 0x01,
    ChanScramble_2700 = 0x02,
    ChanScramble_2800 = 0x03,
    ChanScramble_2900 = 0x04,
    ChanScramble_3000 = 0x05,
    ChanScramble_3100 = 0x06,
    ChanScramble_3200 = 0x07,
    ChanScramble_3300 = 0x08,
    ChanScramble_3400 = 0x09,
    ChanScramble_3500 = 0x0A,
} ChanScrambleMode;
typedef struct ChanChannel {
    unsigned long rxFrequency;
    unsigned int txOffset;
    ChanCode txCode;
    ChanCode rxCode;
    ChanModulation modulation;
    ChanOffsetDirection offsetDirection;
    _Bool preventTxWhileReceiving;
    ChanOutputPower outputPower;
    ChanBandwidth bandwidth;
    _Bool swapRxAndTxFrequencies;
    ChanPttIdMode pttIdMode;
    _Bool enableDtmf;
    ChanFrequencyStep frequencyStep;
    ChanScrambleMode scrambleMode;
    char name[_Chan_CHANNEL_NAME_MAX_CHARACTERS + 1];
    _Bool includeInScanList1;
    _Bool includeInScanList2;
    _Bool enableTxCompander;
    _Bool enableRxCompander;
} ChanChannel;

typedef struct _ChanChannelData {
    uint32_t rxFrequency;
    uint32_t txOffset;
    uint8_t rxCode;
    uint8_t txCode;
    uint8_t txCodeType_rxCodeType;
    uint8_t modulation_offsetDirection;
    uint8_t busyLock_outputPower_bandwidth_reverse;
    uint8_t pttId_dtmf;
    uint8_t step;
    uint8_t scramble;
} _ChanChannelData;

typedef struct Chan {
    _ChanChannelData _data[200];
    char _name[200][16];
    uint8_t _attr[200];
} Chan;

/**
 * *_AUTOFIXED can safely be ignored. It is only for informational purposes.
*/
typedef enum ChanSetChannelErr {
    ChanSetChannelErr_NONE = 0,
    ChanSetChannelErr_MAX_CHANNEL_NUMBER_IS_199 = 2,
    ChanSetChannelErr_MAX_RX_FREQUENCY_IS_1300000000 = 3,
    ChanSetChannelErr_TX_OFFSET_BIGGER_THAN_RX_FREQUENCY = 4,
    ChanSetChannelErr_INVALID_CHANNEL_NAME = 5,
    ChanSetChannelErr_NON_ASCII_CHANNEL_NAME_AUTOFIXED = 6,
} ChanSetChannelErr;
ChanSetChannelErr chanSetChannel(Chan *chan, unsigned int channelNumber, const ChanChannel *ch);

#endif
