#ifndef _LIBQUANSHANG_CHANNELS_CHANNEL_H
#define _LIBQUANSHANG_CHANNELS_CHANNEL_H

#define _qdc_CHANNEL_NAME_MAX_CHARACTERS sizeof(char) * 10 // More cannot fit on radio screen

#define _qdc_Code_TYPE_NONE 0x000
#define _qdc_Code_TYPE_CTCSS 0x100
#define _qdc_Code_TYPE_DCS 0x200
#define _qdc_Code_TYPE_DCS_INVERTED 0x300

#define _qdc_Code_NOTYPE_DCS_23   0x00
#define _qdc_Code_NOTYPE_DCS_25   0x01
#define _qdc_Code_NOTYPE_DCS_26   0x02
#define _qdc_Code_NOTYPE_DCS_31   0x03
#define _qdc_Code_NOTYPE_DCS_32   0x04
#define _qdc_Code_NOTYPE_DCS_36   0x05
#define _qdc_Code_NOTYPE_DCS_43   0x06
#define _qdc_Code_NOTYPE_DCS_47   0x07
#define _qdc_Code_NOTYPE_DCS_51   0x08
#define _qdc_Code_NOTYPE_DCS_53   0x09
#define _qdc_Code_NOTYPE_DCS_54   0x0A
#define _qdc_Code_NOTYPE_DCS_65   0x0B
#define _qdc_Code_NOTYPE_DCS_71   0x0C
#define _qdc_Code_NOTYPE_DCS_72   0x0D
#define _qdc_Code_NOTYPE_DCS_73   0x0E
#define _qdc_Code_NOTYPE_DCS_74   0x0F
#define _qdc_Code_NOTYPE_DCS_114  0x10
#define _qdc_Code_NOTYPE_DCS_115  0x11
#define _qdc_Code_NOTYPE_DCS_116  0x12
#define _qdc_Code_NOTYPE_DCS_122  0x13
#define _qdc_Code_NOTYPE_DCS_125  0x14
#define _qdc_Code_NOTYPE_DCS_131  0x15
#define _qdc_Code_NOTYPE_DCS_132  0x16
#define _qdc_Code_NOTYPE_DCS_134  0x17
#define _qdc_Code_NOTYPE_DCS_143  0x18
#define _qdc_Code_NOTYPE_DCS_145  0x19
#define _qdc_Code_NOTYPE_DCS_152  0x1A
#define _qdc_Code_NOTYPE_DCS_155  0x1B
#define _qdc_Code_NOTYPE_DCS_156  0x1C
#define _qdc_Code_NOTYPE_DCS_162  0x1D
#define _qdc_Code_NOTYPE_DCS_165  0x1E
#define _qdc_Code_NOTYPE_DCS_172  0x1F
#define _qdc_Code_NOTYPE_DCS_174  0x20
#define _qdc_Code_NOTYPE_DCS_205  0x21
#define _qdc_Code_NOTYPE_DCS_212  0x22
#define _qdc_Code_NOTYPE_DCS_223  0x23
#define _qdc_Code_NOTYPE_DCS_225  0x24
#define _qdc_Code_NOTYPE_DCS_226  0x25
#define _qdc_Code_NOTYPE_DCS_243  0x26
#define _qdc_Code_NOTYPE_DCS_244  0x27
#define _qdc_Code_NOTYPE_DCS_245  0x28
#define _qdc_Code_NOTYPE_DCS_246  0x29
#define _qdc_Code_NOTYPE_DCS_251  0x2A
#define _qdc_Code_NOTYPE_DCS_252  0x2B
#define _qdc_Code_NOTYPE_DCS_255  0x2C
#define _qdc_Code_NOTYPE_DCS_261  0x2D
#define _qdc_Code_NOTYPE_DCS_263  0x2E
#define _qdc_Code_NOTYPE_DCS_265  0x2F
#define _qdc_Code_NOTYPE_DCS_266  0x30
#define _qdc_Code_NOTYPE_DCS_271  0x31
#define _qdc_Code_NOTYPE_DCS_274  0x32
#define _qdc_Code_NOTYPE_DCS_306  0x33
#define _qdc_Code_NOTYPE_DCS_311  0x34
#define _qdc_Code_NOTYPE_DCS_315  0x35
#define _qdc_Code_NOTYPE_DCS_325  0x36
#define _qdc_Code_NOTYPE_DCS_331  0x37
#define _qdc_Code_NOTYPE_DCS_332  0x38
#define _qdc_Code_NOTYPE_DCS_343  0x39
#define _qdc_Code_NOTYPE_DCS_346  0x3A
#define _qdc_Code_NOTYPE_DCS_351  0x3B
#define _qdc_Code_NOTYPE_DCS_356  0x3C
#define _qdc_Code_NOTYPE_DCS_364  0x3D
#define _qdc_Code_NOTYPE_DCS_365  0x3E
#define _qdc_Code_NOTYPE_DCS_371  0x3F
#define _qdc_Code_NOTYPE_DCS_411  0x40
#define _qdc_Code_NOTYPE_DCS_412  0x41
#define _qdc_Code_NOTYPE_DCS_413  0x42
#define _qdc_Code_NOTYPE_DCS_423  0x43
#define _qdc_Code_NOTYPE_DCS_431  0x44
#define _qdc_Code_NOTYPE_DCS_432  0x45
#define _qdc_Code_NOTYPE_DCS_445  0x46
#define _qdc_Code_NOTYPE_DCS_446  0x47
#define _qdc_Code_NOTYPE_DCS_452  0x48
#define _qdc_Code_NOTYPE_DCS_454  0x49
#define _qdc_Code_NOTYPE_DCS_455  0x4A
#define _qdc_Code_NOTYPE_DCS_462  0x4B
#define _qdc_Code_NOTYPE_DCS_464  0x4C
#define _qdc_Code_NOTYPE_DCS_465  0x4D
#define _qdc_Code_NOTYPE_DCS_466  0x4E
#define _qdc_Code_NOTYPE_DCS_503  0x4F
#define _qdc_Code_NOTYPE_DCS_506  0x50
#define _qdc_Code_NOTYPE_DCS_516  0x51
#define _qdc_Code_NOTYPE_DCS_523  0x52
#define _qdc_Code_NOTYPE_DCS_526  0x53
#define _qdc_Code_NOTYPE_DCS_532  0x54
#define _qdc_Code_NOTYPE_DCS_546  0x55
#define _qdc_Code_NOTYPE_DCS_565  0x56
#define _qdc_Code_NOTYPE_DCS_606  0x57
#define _qdc_Code_NOTYPE_DCS_612  0x58
#define _qdc_Code_NOTYPE_DCS_624  0x59
#define _qdc_Code_NOTYPE_DCS_627  0x5A
#define _qdc_Code_NOTYPE_DCS_631  0x5B
#define _qdc_Code_NOTYPE_DCS_632  0x5C
#define _qdc_Code_NOTYPE_DCS_654  0x5D
#define _qdc_Code_NOTYPE_DCS_662  0x5E
#define _qdc_Code_NOTYPE_DCS_664  0x5F
#define _qdc_Code_NOTYPE_DCS_703  0x60
#define _qdc_Code_NOTYPE_DCS_712  0x61
#define _qdc_Code_NOTYPE_DCS_723  0x62
#define _qdc_Code_NOTYPE_DCS_731  0x63
#define _qdc_Code_NOTYPE_DCS_732  0x64
#define _qdc_Code_NOTYPE_DCS_734  0x65
#define _qdc_Code_NOTYPE_DCS_743  0x66
#define _qdc_Code_NOTYPE_DCS_754  0x67

#define _qdc_Code_DCS_ENUM_ENTRY(code) \
qdc_Code_DCS_##code = _qdc_Code_TYPE_DCS | _qdc_Code_NOTYPE_DCS_##code, \
qdc_Code_DCS_INVERTED_##code = _qdc_Code_TYPE_DCS_INVERTED | _qdc_Code_NOTYPE_DCS_##code

typedef enum qdc_Code {
    qdc_Code_NONE = _qdc_Code_TYPE_NONE,

    qdc_Code_CTCSS_67 = _qdc_Code_TYPE_CTCSS | 0x00,
    qdc_Code_CTCSS_69_3 = _qdc_Code_TYPE_CTCSS | 0x01,
    qdc_Code_CTCSS_71_9 = _qdc_Code_TYPE_CTCSS | 0x02,
    qdc_Code_CTCSS_74_4 = _qdc_Code_TYPE_CTCSS | 0x03,
    qdc_Code_CTCSS_77 = _qdc_Code_TYPE_CTCSS | 0x04,
    qdc_Code_CTCSS_79_7 = _qdc_Code_TYPE_CTCSS | 0x05,
    qdc_Code_CTCSS_82_5 = _qdc_Code_TYPE_CTCSS | 0x06,
    qdc_Code_CTCSS_85_4 = _qdc_Code_TYPE_CTCSS | 0x07,
    qdc_Code_CTCSS_88_5 = _qdc_Code_TYPE_CTCSS | 0x08,
    qdc_Code_CTCSS_91_5 = _qdc_Code_TYPE_CTCSS | 0x09,
    qdc_Code_CTCSS_94_8 = _qdc_Code_TYPE_CTCSS | 0x0A,
    qdc_Code_CTCSS_97_4 = _qdc_Code_TYPE_CTCSS | 0x0B,
    qdc_Code_CTCSS_100 = _qdc_Code_TYPE_CTCSS | 0x0C,
    qdc_Code_CTCSS_103_5 = _qdc_Code_TYPE_CTCSS | 0x0D,
    qdc_Code_CTCSS_107_2 = _qdc_Code_TYPE_CTCSS | 0x0E,
    qdc_Code_CTCSS_110_9 = _qdc_Code_TYPE_CTCSS | 0x0F,
    qdc_Code_CTCSS_114_8 = _qdc_Code_TYPE_CTCSS | 0x10,
    qdc_Code_CTCSS_118_8 = _qdc_Code_TYPE_CTCSS | 0x11,
    qdc_Code_CTCSS_123 = _qdc_Code_TYPE_CTCSS | 0x12,
    qdc_Code_CTCSS_127_3 = _qdc_Code_TYPE_CTCSS | 0x13,
    qdc_Code_CTCSS_131_8 = _qdc_Code_TYPE_CTCSS | 0x14,
    qdc_Code_CTCSS_136_5 = _qdc_Code_TYPE_CTCSS | 0x15,
    qdc_Code_CTCSS_141_3 = _qdc_Code_TYPE_CTCSS | 0x16,
    qdc_Code_CTCSS_146_2 = _qdc_Code_TYPE_CTCSS | 0x17,
    qdc_Code_CTCSS_151_4 = _qdc_Code_TYPE_CTCSS | 0x18,
    qdc_Code_CTCSS_156_7 = _qdc_Code_TYPE_CTCSS | 0x19,
    qdc_Code_CTCSS_159_8 = _qdc_Code_TYPE_CTCSS | 0x1A,
    qdc_Code_CTCSS_162_2 = _qdc_Code_TYPE_CTCSS | 0x1B,
    qdc_Code_CTCSS_165_5 = _qdc_Code_TYPE_CTCSS | 0x1C,
    qdc_Code_CTCSS_167_9 = _qdc_Code_TYPE_CTCSS | 0x1D,
    qdc_Code_CTCSS_171_3 = _qdc_Code_TYPE_CTCSS | 0x1E,
    qdc_Code_CTCSS_173_8 = _qdc_Code_TYPE_CTCSS | 0x1F,
    qdc_Code_CTCSS_177_3 = _qdc_Code_TYPE_CTCSS | 0x20,
    qdc_Code_CTCSS_179_9 = _qdc_Code_TYPE_CTCSS | 0x21,
    qdc_Code_CTCSS_183_5 = _qdc_Code_TYPE_CTCSS | 0x22,
    qdc_Code_CTCSS_186_2 = _qdc_Code_TYPE_CTCSS | 0x23,
    qdc_Code_CTCSS_189_9 = _qdc_Code_TYPE_CTCSS | 0x24,
    qdc_Code_CTCSS_192_8 = _qdc_Code_TYPE_CTCSS | 0x25,
    qdc_Code_CTCSS_196_6 = _qdc_Code_TYPE_CTCSS | 0x26,
    qdc_Code_CTCSS_199_5 = _qdc_Code_TYPE_CTCSS | 0x27,
    qdc_Code_CTCSS_203_5 = _qdc_Code_TYPE_CTCSS | 0x28,
    qdc_Code_CTCSS_206_5 = _qdc_Code_TYPE_CTCSS | 0x29,
    qdc_Code_CTCSS_210_7 = _qdc_Code_TYPE_CTCSS | 0x2A,
    qdc_Code_CTCSS_218_1 = _qdc_Code_TYPE_CTCSS | 0x2B,
    qdc_Code_CTCSS_225_7 = _qdc_Code_TYPE_CTCSS | 0x2C,
    qdc_Code_CTCSS_229_1 = _qdc_Code_TYPE_CTCSS | 0x2D,
    qdc_Code_CTCSS_233_6 = _qdc_Code_TYPE_CTCSS | 0x2E,
    qdc_Code_CTCSS_241_8 = _qdc_Code_TYPE_CTCSS | 0x2F,
    qdc_Code_CTCSS_250_3 = _qdc_Code_TYPE_CTCSS | 0x30,
    qdc_Code_CTCSS_254_1 = _qdc_Code_TYPE_CTCSS | 0x31,

    _qdc_Code_DCS_ENUM_ENTRY(23),
    _qdc_Code_DCS_ENUM_ENTRY(25),
    _qdc_Code_DCS_ENUM_ENTRY(26),
    _qdc_Code_DCS_ENUM_ENTRY(31),
    _qdc_Code_DCS_ENUM_ENTRY(32),
    _qdc_Code_DCS_ENUM_ENTRY(36),
    _qdc_Code_DCS_ENUM_ENTRY(43),
    _qdc_Code_DCS_ENUM_ENTRY(47),
    _qdc_Code_DCS_ENUM_ENTRY(51),
    _qdc_Code_DCS_ENUM_ENTRY(53),
    _qdc_Code_DCS_ENUM_ENTRY(54),
    _qdc_Code_DCS_ENUM_ENTRY(65),
    _qdc_Code_DCS_ENUM_ENTRY(71),
    _qdc_Code_DCS_ENUM_ENTRY(72),
    _qdc_Code_DCS_ENUM_ENTRY(73),
    _qdc_Code_DCS_ENUM_ENTRY(74),
    _qdc_Code_DCS_ENUM_ENTRY(114),
    _qdc_Code_DCS_ENUM_ENTRY(115),
    _qdc_Code_DCS_ENUM_ENTRY(116),
    _qdc_Code_DCS_ENUM_ENTRY(122),
    _qdc_Code_DCS_ENUM_ENTRY(125),
    _qdc_Code_DCS_ENUM_ENTRY(131),
    _qdc_Code_DCS_ENUM_ENTRY(132),
    _qdc_Code_DCS_ENUM_ENTRY(134),
    _qdc_Code_DCS_ENUM_ENTRY(143),
    _qdc_Code_DCS_ENUM_ENTRY(145),
    _qdc_Code_DCS_ENUM_ENTRY(152),
    _qdc_Code_DCS_ENUM_ENTRY(155),
    _qdc_Code_DCS_ENUM_ENTRY(156),
    _qdc_Code_DCS_ENUM_ENTRY(162),
    _qdc_Code_DCS_ENUM_ENTRY(165),
    _qdc_Code_DCS_ENUM_ENTRY(172),
    _qdc_Code_DCS_ENUM_ENTRY(174),
    _qdc_Code_DCS_ENUM_ENTRY(205),
    _qdc_Code_DCS_ENUM_ENTRY(212),
    _qdc_Code_DCS_ENUM_ENTRY(223),
    _qdc_Code_DCS_ENUM_ENTRY(225),
    _qdc_Code_DCS_ENUM_ENTRY(226),
    _qdc_Code_DCS_ENUM_ENTRY(243),
    _qdc_Code_DCS_ENUM_ENTRY(244),
    _qdc_Code_DCS_ENUM_ENTRY(245),
    _qdc_Code_DCS_ENUM_ENTRY(246),
    _qdc_Code_DCS_ENUM_ENTRY(251),
    _qdc_Code_DCS_ENUM_ENTRY(252),
    _qdc_Code_DCS_ENUM_ENTRY(255),
    _qdc_Code_DCS_ENUM_ENTRY(261),
    _qdc_Code_DCS_ENUM_ENTRY(263),
    _qdc_Code_DCS_ENUM_ENTRY(265),
    _qdc_Code_DCS_ENUM_ENTRY(266),
    _qdc_Code_DCS_ENUM_ENTRY(271),
    _qdc_Code_DCS_ENUM_ENTRY(274),
    _qdc_Code_DCS_ENUM_ENTRY(306),
    _qdc_Code_DCS_ENUM_ENTRY(311),
    _qdc_Code_DCS_ENUM_ENTRY(315),
    _qdc_Code_DCS_ENUM_ENTRY(325),
    _qdc_Code_DCS_ENUM_ENTRY(331),
    _qdc_Code_DCS_ENUM_ENTRY(332),
    _qdc_Code_DCS_ENUM_ENTRY(343),
    _qdc_Code_DCS_ENUM_ENTRY(346),
    _qdc_Code_DCS_ENUM_ENTRY(351),
    _qdc_Code_DCS_ENUM_ENTRY(356),
    _qdc_Code_DCS_ENUM_ENTRY(364),
    _qdc_Code_DCS_ENUM_ENTRY(365),
    _qdc_Code_DCS_ENUM_ENTRY(371),
    _qdc_Code_DCS_ENUM_ENTRY(411),
    _qdc_Code_DCS_ENUM_ENTRY(412),
    _qdc_Code_DCS_ENUM_ENTRY(413),
    _qdc_Code_DCS_ENUM_ENTRY(423),
    _qdc_Code_DCS_ENUM_ENTRY(431),
    _qdc_Code_DCS_ENUM_ENTRY(432),
    _qdc_Code_DCS_ENUM_ENTRY(445),
    _qdc_Code_DCS_ENUM_ENTRY(446),
    _qdc_Code_DCS_ENUM_ENTRY(452),
    _qdc_Code_DCS_ENUM_ENTRY(454),
    _qdc_Code_DCS_ENUM_ENTRY(455),
    _qdc_Code_DCS_ENUM_ENTRY(462),
    _qdc_Code_DCS_ENUM_ENTRY(464),
    _qdc_Code_DCS_ENUM_ENTRY(465),
    _qdc_Code_DCS_ENUM_ENTRY(466),
    _qdc_Code_DCS_ENUM_ENTRY(503),
    _qdc_Code_DCS_ENUM_ENTRY(506),
    _qdc_Code_DCS_ENUM_ENTRY(516),
    _qdc_Code_DCS_ENUM_ENTRY(523),
    _qdc_Code_DCS_ENUM_ENTRY(526),
    _qdc_Code_DCS_ENUM_ENTRY(532),
    _qdc_Code_DCS_ENUM_ENTRY(546),
    _qdc_Code_DCS_ENUM_ENTRY(565),
    _qdc_Code_DCS_ENUM_ENTRY(606),
    _qdc_Code_DCS_ENUM_ENTRY(612),
    _qdc_Code_DCS_ENUM_ENTRY(624),
    _qdc_Code_DCS_ENUM_ENTRY(627),
    _qdc_Code_DCS_ENUM_ENTRY(631),
    _qdc_Code_DCS_ENUM_ENTRY(632),
    _qdc_Code_DCS_ENUM_ENTRY(654),
    _qdc_Code_DCS_ENUM_ENTRY(662),
    _qdc_Code_DCS_ENUM_ENTRY(664),
    _qdc_Code_DCS_ENUM_ENTRY(703),
    _qdc_Code_DCS_ENUM_ENTRY(712),
    _qdc_Code_DCS_ENUM_ENTRY(723),
    _qdc_Code_DCS_ENUM_ENTRY(731),
    _qdc_Code_DCS_ENUM_ENTRY(732),
    _qdc_Code_DCS_ENUM_ENTRY(734),
    _qdc_Code_DCS_ENUM_ENTRY(743),
    _qdc_Code_DCS_ENUM_ENTRY(754)
} qdc_Code;

typedef enum qdc_Modulation {
    qdc_Modulation_FM = 0x0,
    qdc_Modulation_AM = 0x1,
    qdc_Modulation_USB = 0x2,
} qdc_Modulation;

typedef enum qdc_OffsetDirection {
    qdc_OffsetDirection_NONE = 0x0,
    qdc_OffsetDirection_PLUS = 0x1,
    qdc_OffsetDirection_MINUS = 0x2,
} qdc_OffsetDirection;

typedef enum qdc_OutputPower {
    qdc_OutputPower_LOW = 0x0,
    qdc_OutputPower_MEDIUM = 0x1,
    qdc_OutputPower_HIGH = 0x2,
} qdc_OutputPower;

typedef enum qdc_Bandwidth {
    qdc_Bandwidth_WIDE = 0x0,
    qdc_Bandwidth_NARROW = 0x1,
} qdc_Bandwidth;

typedef enum qdc_PttIdMode {
    qdc_PttId_NONE = 0b000,
    qdc_PttId_BEGIN = 0b001,
    qdc_PttId_END = 0b010,
    qdc_PttId_BEGIN_END = 0b011,
    qdc_PttId_APOLLO_QUINDAR = 0b100,
} qdc_PttIdMode;

typedef enum qdc_FrequencyStep {
    qdc_FrequencyStep_0_01 = 0x07,
    qdc_FrequencyStep_0_05 = 0x08,
    qdc_FrequencyStep_0_1 = 0x09,
    qdc_FrequencyStep_0_25 = 0x0A,
    qdc_FrequencyStep_0_5 = 0x0B,
    qdc_FrequencyStep_1 = 0x0C,
    qdc_FrequencyStep_1_25 = 0x0D,
    qdc_FrequencyStep_2_5 = 0x00,
    qdc_FrequencyStep_5 = 0x01,
    qdc_FrequencyStep_6_25 = 0x02,
    qdc_FrequencyStep_8_33 = 0x06,
    qdc_FrequencyStep_10 = 0x03,
    qdc_FrequencyStep_12_5 = 0x04,
    qdc_FrequencyStep_15 = 0x0E,
    qdc_FrequencyStep_25 = 0x05,
    qdc_FrequencyStep_30 = 0x0F,
    qdc_FrequencyStep_50 = 0x10,
    qdc_FrequencyStep_100 = 0x11,
    qdc_FrequencyStep_125 = 0x12,
    qdc_FrequencyStep_250 = 0x13,
    qdc_FrequencyStep_500 = 0x14,
} qdc_FrequencyStep;

typedef enum qdc_ScrambleMode {
    qdc_Scramble_NONE = 0x00,
    qdc_Scramble_2600 = 0x01,
    qdc_Scramble_2700 = 0x02,
    qdc_Scramble_2800 = 0x03,
    qdc_Scramble_2900 = 0x04,
    qdc_Scramble_3000 = 0x05,
    qdc_Scramble_3100 = 0x06,
    qdc_Scramble_3200 = 0x07,
    qdc_Scramble_3300 = 0x08,
    qdc_Scramble_3400 = 0x09,
    qdc_Scramble_3500 = 0x0A,
} qdc_ScrambleMode;

typedef struct qdc_Channel {
    unsigned long rxFrequency;
    unsigned long txOffset;
    qdc_Code txCode;
    qdc_Code rxCode;
    qdc_Modulation modulation;
    qdc_OffsetDirection offsetDirection;
    _Bool preventTxWhileReceiving;
    qdc_OutputPower outputPower;
    qdc_Bandwidth bandwidth;
    _Bool swapRxAndTxFrequencies;
    qdc_PttIdMode pttIdMode;
    _Bool enableDtmf;
    qdc_FrequencyStep frequencyStep;
    qdc_ScrambleMode scrambleMode;
    char name[_qdc_CHANNEL_NAME_MAX_CHARACTERS + 1];
    _Bool includeInScanList1;
    _Bool includeInScanList2;
    _Bool enableTxCompander;
    _Bool enableRxCompander;
} qdc_Channel;

#endif
