// Copyright 2018 Google Inc. All Rights Reserved.

#if !defined(__IMX227_SEQ_H__)
#define __IMX227_SEQ_H__

static acam_reg_t init[] = {
    //wait command - address is 0xFFFF
    { 0xFFFF, 20 },
    //stop sequence - address is 0x0000
    { 0x0000, 0x0000, 0x0000, 0x0000 }
};

static acam_reg_t setting_2200_2720_2lane_1g_30fps[] = {
	{0x0136, 0x18, 0xFF, 1},
	{0x0137, 0x00, 0xFF, 1},
	{0xAE20, 0x00, 0xFF, 1},
	{0x463B, 0x30, 0xFF, 1},
	{0x463E, 0x05, 0xFF, 1},
	{0x4612, 0x66, 0xFF, 1},
	{0x4815, 0x65, 0xFF, 1},
	{0x4991, 0x00, 0xFF, 1},
	{0x4992, 0x01, 0xFF, 1},
	{0x4993, 0xFF, 0xFF, 1},
	{0x458B, 0x00, 0xFF, 1},
	{0x452A, 0x02, 0xFF, 1},
	{0x4A7C, 0x00, 0xFF, 1},
	{0x4A7D, 0x1C, 0xFF, 1},
	{0x4A7E, 0x00, 0xFF, 1},
	{0x4A7F, 0x17, 0xFF, 1},
	{0x462C, 0x2E, 0xFF, 1},
	{0x461B, 0x28, 0xFF, 1},
	{0x4663, 0x29, 0xFF, 1},
	{0x461A, 0x7C, 0xFF, 1},
	{0x4619, 0x28, 0xFF, 1},
	{0x4667, 0x22, 0xFF, 1},
	{0x466B, 0x23, 0xFF, 1},
	{0x4D16, 0x00, 0xFF, 1},
	{0x6204, 0x01, 0xFF, 1},
	{0x6209, 0x00, 0xFF, 1},
	{0x621F, 0x01, 0xFF, 1},
	{0x621E, 0x10, 0xFF, 1},
	{0x48E3, 0x82, 0xFF, 1},
	{0x0114, 0x01, 0xFF, 1},
	{0x0340, 0x0A, 0xFF, 1},
	{0x0341, 0xE0, 0xFF, 1},
	{0x0342, 0x10, 0xFF, 1},
	{0x0343, 0x00, 0xFF, 1},
	{0x0344, 0x00, 0xFF, 1},
	{0x0345, 0x64, 0xFF, 1},
	{0x0346, 0x00, 0xFF, 1},
	{0x0347, 0x00, 0xFF, 1},
	{0x0348, 0x08, 0xFF, 1},
	{0x0349, 0xFB, 0xFF, 1},
	{0x034A, 0x0A, 0xFF, 1},
	{0x034B, 0x9F, 0xFF, 1},
	{0x0381, 0x01, 0xFF, 1},
	{0x0383, 0x01, 0xFF, 1},
	{0x0385, 0x01, 0xFF, 1},
	{0x0387, 0x01, 0xFF, 1},
	{0x0900, 0x00, 0xFF, 1},
	{0x0901, 0x00, 0xFF, 1},
	{0x0112, 0x0A, 0xFF, 1},
	{0x0113, 0x0A, 0xFF, 1},
	{0x034C, 0x08, 0xFF, 1},
	{0x034D, 0x98, 0xFF, 1},
	{0x034E, 0x0A, 0xFF, 1},
	{0x034F, 0xA0, 0xFF, 1},
	{0x0408, 0x00, 0xFF, 1},
	{0x0409, 0x00, 0xFF, 1},
	{0x040A, 0x00, 0xFF, 1},
	{0x040B, 0x00, 0xFF, 1},
	{0x040C, 0x08, 0xFF, 1},
	{0x040D, 0x98, 0xFF, 1},
	{0x040E, 0x0A, 0xFF, 1},
	{0x040F, 0xA0, 0xFF, 1},
	{0x0301, 0x07, 0xFF, 1},
	{0x0303, 0x02, 0xFF, 1},
	{0x0305, 0x04, 0xFF, 1},
	{0x0306, 0x00, 0xFF, 1},
	{0x0307, 0xC8, 0xFF, 1},
	{0x0309, 0x0A, 0xFF, 1},
	{0x030B, 0x01, 0xFF, 1},
	{0x030D, 0x0F, 0xFF, 1},
	{0x030E, 0x02, 0xFF, 1},
	{0x030F, 0x71, 0xFF, 1},
	{0x0310, 0x01, 0xFF, 1},
	{0x0820, 0x07, 0xFF, 1},
	{0x0821, 0xD0, 0xFF, 1},
	{0x0822, 0x00, 0xFF, 1},
	{0x0823, 0x00, 0xFF, 1},
	{0x3000, 0x00, 0xFF, 1},
	{0x3002, 0x01, 0xFF, 1},
	{0x3003, 0x35, 0xFF, 1},
	{0x6259, 0x06, 0xFF, 1},
	{0x0202, 0x0A, 0xFF, 1},// int time
	{0x0203, 0xDE, 0xFF, 1},
	{0x0204, 0x00, 0xFF, 1},
	{0x0205, 0xC0, 0xFF, 1},//a gain
	{0x020E, 0x01, 0xFF, 1},
	{0x020F, 0x00, 0xFF, 1},
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

static acam_reg_t setting_2200_2720_2lane_1g_30fps_test_pattern[] = {
	{0x0136, 0x18, 0xFF, 1},
	{0x0137, 0x00, 0xFF, 1},
	{0xAE20, 0x00, 0xFF, 1},
	{0x463B, 0x30, 0xFF, 1},
	{0x463E, 0x05, 0xFF, 1},
	{0x4612, 0x66, 0xFF, 1},
	{0x4815, 0x65, 0xFF, 1},
	{0x4991, 0x00, 0xFF, 1},
	{0x4992, 0x01, 0xFF, 1},
	{0x4993, 0xFF, 0xFF, 1},
	{0x458B, 0x00, 0xFF, 1},
	{0x452A, 0x02, 0xFF, 1},
	{0x4A7C, 0x00, 0xFF, 1},
	{0x4A7D, 0x1C, 0xFF, 1},
	{0x4A7E, 0x00, 0xFF, 1},
	{0x4A7F, 0x17, 0xFF, 1},
	{0x462C, 0x2E, 0xFF, 1},
	{0x461B, 0x28, 0xFF, 1},
	{0x4663, 0x29, 0xFF, 1},
	{0x461A, 0x7C, 0xFF, 1},
	{0x4619, 0x28, 0xFF, 1},
	{0x4667, 0x22, 0xFF, 1},
	{0x466B, 0x23, 0xFF, 1},
	{0x4D16, 0x00, 0xFF, 1},
	{0x6204, 0x01, 0xFF, 1},
	{0x6209, 0x00, 0xFF, 1},
	{0x621F, 0x01, 0xFF, 1},
	{0x621E, 0x10, 0xFF, 1},
	{0x48E3, 0x82, 0xFF, 1},
	{0x0114, 0x01, 0xFF, 1},
	{0x0340, 0x0A, 0xFF, 1},
	{0x0341, 0xE0, 0xFF, 1},
	{0x0342, 0x10, 0xFF, 1},
	{0x0343, 0x00, 0xFF, 1},
	{0x0344, 0x00, 0xFF, 1},
	{0x0345, 0x64, 0xFF, 1},
	{0x0346, 0x00, 0xFF, 1},
	{0x0347, 0x00, 0xFF, 1},
	{0x0348, 0x08, 0xFF, 1},
	{0x0349, 0xFB, 0xFF, 1},
	{0x034A, 0x0A, 0xFF, 1},
	{0x034B, 0x9F, 0xFF, 1},
	{0x0381, 0x01, 0xFF, 1},
	{0x0383, 0x01, 0xFF, 1},
	{0x0385, 0x01, 0xFF, 1},
	{0x0387, 0x01, 0xFF, 1},
	{0x0900, 0x00, 0xFF, 1},
	{0x0901, 0x00, 0xFF, 1},
	{0x0112, 0x0A, 0xFF, 1},
	{0x0113, 0x0A, 0xFF, 1},
	{0x034C, 0x08, 0xFF, 1},
	{0x034D, 0x98, 0xFF, 1},
	{0x034E, 0x0A, 0xFF, 1},
	{0x034F, 0xA0, 0xFF, 1},
	{0x0408, 0x00, 0xFF, 1},
	{0x0409, 0x00, 0xFF, 1},
	{0x040A, 0x00, 0xFF, 1},
	{0x040B, 0x00, 0xFF, 1},
	{0x040C, 0x08, 0xFF, 1},
	{0x040D, 0x98, 0xFF, 1},
	{0x040E, 0x0A, 0xFF, 1},
	{0x040F, 0xA0, 0xFF, 1},
	{0x0301, 0x07, 0xFF, 1},
	{0x0303, 0x02, 0xFF, 1},
	{0x0305, 0x04, 0xFF, 1},
	{0x0306, 0x00, 0xFF, 1},
	{0x0307, 0xC8, 0xFF, 1},
	{0x0309, 0x0A, 0xFF, 1},
	{0x030B, 0x01, 0xFF, 1},
	{0x030D, 0x0F, 0xFF, 1},
	{0x030E, 0x02, 0xFF, 1},
	{0x030F, 0x71, 0xFF, 1},
	{0x0310, 0x01, 0xFF, 1},
	{0x0820, 0x07, 0xFF, 1},
	{0x0821, 0xD0, 0xFF, 1},
	{0x0822, 0x00, 0xFF, 1},
	{0x0823, 0x00, 0xFF, 1},
	{0x3000, 0x00, 0xFF, 1},
	{0x3002, 0x01, 0xFF, 1},
	{0x3003, 0x35, 0xFF, 1},
	{0x6259, 0x06, 0xFF, 1},
	{0x0600, 0x00, 0xFF, 1},
	{0x0601, 0x03, 0xFF, 1},
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

static acam_reg_t setting_2200_2720_2lane_960m_30fps_test_pattern[] = {
	{0x0136, 0x18, 0xFF, 1},
	{0x0137, 0x00, 0xFF, 1},
	{0xAE20, 0x00, 0xFF, 1},
	{0x463B, 0x30, 0xFF, 1},
	{0x463E, 0x05, 0xFF, 1},
	{0x4612, 0x66, 0xFF, 1},
	{0x4815, 0x65, 0xFF, 1},
	{0x4991, 0x00, 0xFF, 1},
	{0x4992, 0x01, 0xFF, 1},
	{0x4993, 0xFF, 0xFF, 1},
	{0x458B, 0x00, 0xFF, 1},
	{0x452A, 0x02, 0xFF, 1},
	{0x4A7C, 0x00, 0xFF, 1},
	{0x4A7D, 0x1C, 0xFF, 1},
	{0x4A7E, 0x00, 0xFF, 1},
	{0x4A7F, 0x17, 0xFF, 1},
	{0x462C, 0x2E, 0xFF, 1},
	{0x461B, 0x28, 0xFF, 1},
	{0x4663, 0x29, 0xFF, 1},
	{0x461A, 0x7C, 0xFF, 1},
	{0x4619, 0x28, 0xFF, 1},
	{0x4667, 0x22, 0xFF, 1},
	{0x466B, 0x23, 0xFF, 1},
	{0x4D16, 0x00, 0xFF, 1},
	{0x6204, 0x01, 0xFF, 1},
	{0x6209, 0x00, 0xFF, 1},
	{0x621F, 0x01, 0xFF, 1},
	{0x621E, 0x10, 0xFF, 1},
	{0x48E3, 0x82, 0xFF, 1},
	{0x0114, 0x01, 0xFF, 1},
	{0x0340, 0x0A, 0xFF, 1},
	{0x0341, 0xC0, 0xFF, 1},
	{0x0342, 0x10, 0xFF, 1},
	{0x0343, 0x38, 0xFF, 1},
	{0x0344, 0x00, 0xFF, 1},
	{0x0345, 0x64, 0xFF, 1},
	{0x0346, 0x00, 0xFF, 1},
	{0x0347, 0x00, 0xFF, 1},
	{0x0348, 0x08, 0xFF, 1},
	{0x0349, 0xFB, 0xFF, 1},
	{0x034A, 0x0A, 0xFF, 1},
	{0x034B, 0x9F, 0xFF, 1},
	{0x0381, 0x01, 0xFF, 1},
	{0x0383, 0x01, 0xFF, 1},
	{0x0385, 0x01, 0xFF, 1},
	{0x0387, 0x01, 0xFF, 1},
	{0x0900, 0x00, 0xFF, 1},
	{0x0901, 0x00, 0xFF, 1},
	{0x0112, 0x0A, 0xFF, 1},
	{0x0113, 0x0A, 0xFF, 1},
	{0x034C, 0x08, 0xFF, 1},
	{0x034D, 0x98, 0xFF, 1},
	{0x034E, 0x0A, 0xFF, 1},
	{0x034F, 0xA0, 0xFF, 1},
	{0x0408, 0x00, 0xFF, 1},
	{0x0409, 0x00, 0xFF, 1},
	{0x040A, 0x00, 0xFF, 1},
	{0x040B, 0x00, 0xFF, 1},
	{0x040C, 0x08, 0xFF, 1},
	{0x040D, 0x98, 0xFF, 1},
	{0x040E, 0x0A, 0xFF, 1},
	{0x040F, 0xA0, 0xFF, 1},
	{0x0301, 0x07, 0xFF, 1},
	{0x0303, 0x02, 0xFF, 1},
	{0x0305, 0x04, 0xFF, 1},
	{0x0306, 0x00, 0xFF, 1},
	{0x0307, 0xC8, 0xFF, 1},
	{0x0309, 0x0A, 0xFF, 1},
	{0x030B, 0x01, 0xFF, 1},
	{0x030D, 0x0F, 0xFF, 1},
	{0x030E, 0x02, 0xFF, 1},
	{0x030F, 0x58, 0xFF, 1},
	{0x0310, 0x01, 0xFF, 1},
	{0x0820, 0x07, 0xFF, 1},
	{0x0821, 0x80, 0xFF, 1},
	{0x0822, 0x00, 0xFF, 1},
	{0x0823, 0x00, 0xFF, 1},
	{0x3000, 0x00, 0xFF, 1},
	{0x3002, 0x01, 0xFF, 1},
	{0x3003, 0x35, 0xFF, 1},
	{0x6259, 0x06, 0xFF, 1},
	{0x0600, 0x00, 0xFF, 1},
	{0x0601, 0x03, 0xFF, 1},
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

static acam_reg_t setting_1920_1080_2lane_1g_30fps_test_pattern[] = {
	{0x0136, 0x18, 0xFF, 1},
	{0x0137, 0x00, 0xFF, 1},
	{0xAE20, 0x00, 0xFF, 1},
	{0x463B, 0x30, 0xFF, 1},
	{0x463E, 0x05, 0xFF, 1},
	{0x4612, 0x66, 0xFF, 1},
	{0x4815, 0x65, 0xFF, 1},
	{0x4991, 0x00, 0xFF, 1},
	{0x4992, 0x01, 0xFF, 1},
	{0x4993, 0xFF, 0xFF, 1},
	{0x458B, 0x00, 0xFF, 1},
	{0x452A, 0x02, 0xFF, 1},
	{0x4A7C, 0x00, 0xFF, 1},
	{0x4A7D, 0x1C, 0xFF, 1},
	{0x4A7E, 0x00, 0xFF, 1},
	{0x4A7F, 0x17, 0xFF, 1},
	{0x462C, 0x2E, 0xFF, 1},
	{0x461B, 0x28, 0xFF, 1},
	{0x4663, 0x29, 0xFF, 1},
	{0x461A, 0x7C, 0xFF, 1},
	{0x4619, 0x28, 0xFF, 1},
	{0x4667, 0x22, 0xFF, 1},
	{0x466B, 0x23, 0xFF, 1},
	{0x4D16, 0x00, 0xFF, 1},
	{0x6204, 0x01, 0xFF, 1},
	{0x6209, 0x00, 0xFF, 1},
	{0x621F, 0x01, 0xFF, 1},
	{0x621E, 0x10, 0xFF, 1},
	{0x48E3, 0x82, 0xFF, 1},
	{0x0114, 0x01, 0xFF, 1},
	{0x0340, 0x06, 0xFF, 1},
	{0x0341, 0x08, 0xFF, 1},
	{0x0342, 0x0E, 0xFF, 1},
	{0x0343, 0x70, 0xFF, 1},
	{0x0344, 0x00, 0xFF, 1},
	{0x0345, 0xF0, 0xFF, 1},
	{0x0346, 0x03, 0xFF, 1},
	{0x0347, 0x34, 0xFF, 1},
	{0x0348, 0x08, 0xFF, 1},
	{0x0349, 0x6F, 0xFF, 1},
	{0x034A, 0x07, 0xFF, 1},
	{0x034B, 0x6B, 0xFF, 1},
	{0x0381, 0x01, 0xFF, 1},
	{0x0383, 0x01, 0xFF, 1},
	{0x0385, 0x01, 0xFF, 1},
	{0x0387, 0x01, 0xFF, 1},
	{0x0900, 0x00, 0xFF, 1},
	{0x0901, 0x00, 0xFF, 1},
	{0x0112, 0x0A, 0xFF, 1},
	{0x0113, 0x0A, 0xFF, 1},
	{0x034C, 0x07, 0xFF, 1},
	{0x034D, 0x80, 0xFF, 1},
	{0x034E, 0x04, 0xFF, 1},
	{0x034F, 0x38, 0xFF, 1},
	{0x0408, 0x00, 0xFF, 1},
	{0x0409, 0x00, 0xFF, 1},
	{0x040A, 0x00, 0xFF, 1},
	{0x040B, 0x00, 0xFF, 1},
	{0x040C, 0x07, 0xFF, 1},
	{0x040D, 0x80, 0xFF, 1},
	{0x040E, 0x04, 0xFF, 1},
	{0x040F, 0x38, 0xFF, 1},
	{0x0301, 0x07, 0xFF, 1},
	{0x0303, 0x02, 0xFF, 1},
	{0x0305, 0x04, 0xFF, 1},
	{0x0306, 0x00, 0xFF, 1},
	{0x0307, 0xC8, 0xFF, 1},
	{0x0309, 0x0A, 0xFF, 1},
	{0x030B, 0x01, 0xFF, 1},
	{0x030D, 0x0F, 0xFF, 1},
	{0x030E, 0x02, 0xFF, 1},
	{0x030F, 0x71, 0xFF, 1},
	{0x0310, 0x01, 0xFF, 1},
	{0x0820, 0x07, 0xFF, 1},
	{0x0821, 0xD0, 0xFF, 1},
	{0x0822, 0x00, 0xFF, 1},
	{0x0823, 0x00, 0xFF, 1},
	{0x3000, 0x00, 0xFF, 1},
	{0x3002, 0x01, 0xFF, 1},
	{0x3003, 0x04, 0xFF, 1},
	{0x6259, 0x05, 0xFF, 1},
	{0x0600, 0x00, 0xFF, 1},
	{0x0601, 0x04, 0xFF, 1},
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

static acam_reg_t setting_1920_1080_2lane_1g_30fps[] = {
	{0x0136, 0x18, 0xFF, 1},
	{0x0137, 0x00, 0xFF, 1},
	{0xAE20, 0x00, 0xFF, 1},
	{0x463B, 0x30, 0xFF, 1},
	{0x463E, 0x05, 0xFF, 1},
	{0x4612, 0x66, 0xFF, 1},
	{0x4815, 0x65, 0xFF, 1},
	{0x4991, 0x00, 0xFF, 1},
	{0x4992, 0x01, 0xFF, 1},
	{0x4993, 0xFF, 0xFF, 1},
	{0x458B, 0x00, 0xFF, 1},
	{0x452A, 0x02, 0xFF, 1},
	{0x4A7C, 0x00, 0xFF, 1},
	{0x4A7D, 0x1C, 0xFF, 1},
	{0x4A7E, 0x00, 0xFF, 1},
	{0x4A7F, 0x17, 0xFF, 1},
	{0x462C, 0x2E, 0xFF, 1},
	{0x461B, 0x28, 0xFF, 1},
	{0x4663, 0x29, 0xFF, 1},
	{0x461A, 0x7C, 0xFF, 1},
	{0x4619, 0x28, 0xFF, 1},
	{0x4667, 0x22, 0xFF, 1},
	{0x466B, 0x23, 0xFF, 1},
	{0x4D16, 0x00, 0xFF, 1},
	{0x6204, 0x01, 0xFF, 1},
	{0x6209, 0x00, 0xFF, 1},
	{0x621F, 0x01, 0xFF, 1},
	{0x621E, 0x10, 0xFF, 1},
	{0x48E3, 0x82, 0xFF, 1},
	{0x0114, 0x01, 0xFF, 1},
	{0x0340, 0x06, 0xFF, 1},
	{0x0341, 0x08, 0xFF, 1},
	{0x0342, 0x0E, 0xFF, 1},
	{0x0343, 0x70, 0xFF, 1},
	{0x0344, 0x00, 0xFF, 1},
	{0x0345, 0xF0, 0xFF, 1},
	{0x0346, 0x03, 0xFF, 1},
	{0x0347, 0x34, 0xFF, 1},
	{0x0348, 0x08, 0xFF, 1},
	{0x0349, 0x6F, 0xFF, 1},
	{0x034A, 0x07, 0xFF, 1},
	{0x034B, 0x6B, 0xFF, 1},
	{0x0381, 0x01, 0xFF, 1},
	{0x0383, 0x01, 0xFF, 1},
	{0x0385, 0x01, 0xFF, 1},
	{0x0387, 0x01, 0xFF, 1},
	{0x0900, 0x00, 0xFF, 1},
	{0x0901, 0x00, 0xFF, 1},
	{0x0112, 0x0A, 0xFF, 1},
	{0x0113, 0x0A, 0xFF, 1},
	{0x034C, 0x07, 0xFF, 1},
	{0x034D, 0x80, 0xFF, 1},
	{0x034E, 0x04, 0xFF, 1},
	{0x034F, 0x38, 0xFF, 1},
	{0x0408, 0x00, 0xFF, 1},
	{0x0409, 0x00, 0xFF, 1},
	{0x040A, 0x00, 0xFF, 1},
	{0x040B, 0x00, 0xFF, 1},
	{0x040C, 0x07, 0xFF, 1},
	{0x040D, 0x80, 0xFF, 1},
	{0x040E, 0x04, 0xFF, 1},
	{0x040F, 0x38, 0xFF, 1},
	{0x0301, 0x07, 0xFF, 1},
	{0x0303, 0x02, 0xFF, 1},
	{0x0305, 0x04, 0xFF, 1},
	{0x0306, 0x00, 0xFF, 1},
	{0x0307, 0xC8, 0xFF, 1},
	{0x0309, 0x0A, 0xFF, 1},
	{0x030B, 0x01, 0xFF, 1},
	{0x030D, 0x0F, 0xFF, 1},
	{0x030E, 0x02, 0xFF, 1},
	{0x030F, 0x71, 0xFF, 1},
	{0x0310, 0x01, 0xFF, 1},
	{0x0820, 0x07, 0xFF, 1},
	{0x0821, 0xD0, 0xFF, 1},
	{0x0822, 0x00, 0xFF, 1},
	{0x0823, 0x00, 0xFF, 1},
	{0x3000, 0x00, 0xFF, 1},
	{0x3002, 0x01, 0xFF, 1},
	{0x3003, 0x04, 0xFF, 1},
	{0x6259, 0x05, 0xFF, 1},
	{0x0202, 0x0C, 0xFF, 1},//int time
	{0x0203, 0x0E, 0xFF, 1},
	{0x0204, 0x00, 0xFF, 1},
	{0x0205, 0xE0, 0xFF, 1},//a gain
	{0x020E, 0x01, 0xFF, 1},
	{0x020F, 0x00, 0xFF, 1},
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};



static acam_reg_t settings_context_imx227[] = {
    { 0x19000, 0xf000f0L, 0xfff0fff,4 },
    { 0x1ae7c, 0xb2b4b4c8L, 0x0,4 },
    { 0x1b28c, 0x0L, 0x0,2 },
    { 0x1abb0, 0x1L, 0x31,1 },
    { 0x19004, 0xf000L, 0xfffff,4 },
    { 0x1b104, 0x0L, 0xfff,2 },
    { 0x19008, 0x6000180L, 0xfff0fff,4 },
    { 0x192ac, 0xffffL, 0x0,2 },
    { 0x1bbac, 0x1L, 0x77,1 },
    { 0x1900c, 0xc000600L, 0xfff0fff,4 },
    { 0x19010, 0x1800180L, 0xfff0fff,4 },
    { 0x19358, 0x6eaL, 0x0,2 },
    { 0x19014, 0x1000100L, 0xfff0fff,4 },
    { 0x1b144, 0xfffL, 0xfff,2 },
    { 0x1b038, 0x5dcL, 0xfff,2 },
    { 0x19018, 0x100L, 0xfff,2 },
    { 0x1b004, 0x40L, 0xfff0fff,4 },
    { 0x1aeb0, 0x81d461cL, 0x0,4 },
    { 0x1901c, 0x80000L, 0xfffff,4 },
    { 0x1b0e8, 0x800L, 0xfff,2 },
    { 0x1b138, 0xfffL, 0xfff,2 },
    { 0x19020, 0x21400L, 0x3f3f3f,4 },
    { 0x192b0, 0x400L, 0x0,2 },
    { 0x19024, 0x2000040L, 0xfff0fff,4 },
    { 0x1b0f4, 0x333L, 0x0,2 },
    { 0x1b0d8, 0x0L, 0x0,2 },
    { 0x19028, 0x0L, 0x3fffff,4 },
    { 0x1935c, 0x0L, 0x0,4 },
    { 0x1902c, 0x0L, 0xfff00ff,4 },
    { 0x1c0f0, 0x4380780L, 0x0,4 },
    { 0x1b2cc, 0x0L, 0xfff,2 },
    { 0x1ac20, 0x100L, 0xfff,2 },
    { 0x19030, 0x0L, 0x3fffff,4 },
    { 0x1b008, 0x200L, 0xfff,2 },
    { 0x1b100, 0xfbd8L, 0x0,2 },
    { 0x19034, 0x0L, 0xfffff,4 },
    { 0x1b028, 0xc83L, 0xfff,2 },
    { 0x19038, 0x0L, 0xfff,2 },
    { 0x1abb4, 0x400040L, 0x7f007f,4 },
    { 0x1b2b0, 0x1ffL, 0xfff,2 },
    { 0x1903c, 0x0L, 0x3fffff,4 },
    { 0x1afe4, 0x10a5L, 0x1fff,2 },
    { 0x1c070, 0x0L, 0xfff0fff,4 },
    { 0x1b128, 0x0L, 0xfff,2 },
    { 0x19040, 0x0L, 0xfffff,4 },
    { 0x1ac60, 0x0L, 0xfffff,4 },
    { 0x19044, 0x0L, 0xfff0fff,4 },
    { 0x1b048, 0xfffL, 0xfff,2 },
    { 0x1c124, 0x0L, 0x1ff,2 },
    { 0x19048, 0x400100L, 0xff0fff,4 },
    { 0x1b00c, 0xf8L, 0xfff0fff,4 },
    { 0x1b0d4, 0x1L, 0x1,1 },
    { 0x1904c, 0x3L, 0x10107,4 },
    { 0x19050, 0xdac0dacL, 0xfff0fff,4 },
    { 0x1abb8, 0x10000040L, 0x1fff1fff,4 },
    { 0x19054, 0xdac0dacL, 0xfff0fff,4 },
    { 0x1c1e0, 0x100L, 0xfff,2 },
    { 0x1ac5c, 0x0L, 0x3,1 },
    { 0x19058, 0x8000800L, 0xfff0fff,4 },
    { 0x1b2d0, 0xfffL, 0xfff,2 },
    { 0x1b2a0, 0x3ffL, 0x3ff,2 },
    { 0x1905c, 0x200800L, 0xfff0fff,4 },
    { 0x1afcc, 0x300L, 0xfff,2 },
    { 0x1b118, 0xe7b4L, 0x0,2 },
    { 0x19060, 0x800040L, 0xfff0fff,4 },
    { 0x1b010, 0x7adL, 0xfff,2 },
    { 0x1ac64, 0x1000100L, 0xfff0fff,4 },
    { 0x1ac54, 0x100L, 0x3ffff,4 },
    { 0x1ac68, 0x1000100L, 0xfff0fff,4 },
    { 0x1abbc, 0x0L, 0x1,1 },
    { 0x1926c, 0x100L, 0x1fff,2 },
    { 0x1b148, 0xffffL, 0x0,2 },
    { 0x1ac3c, 0x700202L, 0x3ff0303,4 },
    { 0x19270, 0xf000L, 0xfffff,4 },
    { 0x19368, 0x1e02L, 0xff07,2 },
    { 0x1c08c, 0x6a40000L, 0xffff00ff,4 },
    { 0x19274, 0x0L, 0xfffff,4 },
    { 0x1aecc, 0x3330333L, 0x1fff1fff,4 },
    { 0x1ac4c, 0xb4dc0a01L, 0xffff0f01,4 },
    { 0x19278, 0x0L, 0xfffff,4 },
    { 0x1b014, 0x320L, 0xfff0fff,4 },
    { 0x1b290, 0x5L, 0x0,2 },
    { 0x1927c, 0x0L, 0xfffff,4 },
    { 0x1b108, 0xf0L, 0xfff,2 },
    { 0x19280, 0x0L, 0xfffff,4 },
    { 0x1abc0, 0x7d0L, 0x0,2 },
    { 0x1b160, 0x100L, 0xfff,2 },
    { 0x19284, 0xf000L, 0xfffff,4 },
    { 0x1ac44, 0xffff0000L, 0x0,4 },
    { 0x1ac24, 0x0L, 0xfffff,4 },
    { 0x18e88, 0x4380780L, 0x0,4 },
    { 0x1936c, 0x0L, 0x0,2 },
    { 0x18e8c, 0x0L, 0x3030303,4 },
    { 0x1b03c, 0x190L, 0xfff0fff,4 },
    { 0x19290, 0x0L, 0x0,2 },
    { 0x1b018, 0x960L, 0xfff,2 },
    { 0x1aeb4, 0x1f8bL, 0x1fff,2 },
    { 0x19294, 0xfc300aaL, 0xfff0fff,4 },
    { 0x1b2bc, 0xc4L, 0xfff,2 },
    { 0x1b13c, 0xffffL, 0x0,2 },
    { 0x19298, 0x8000L, 0xffffff,4 },
    { 0x1abc4, 0xf000L, 0xfffff,4 },
    { 0x1b280, 0xd0L, 0xfff,2 },
    { 0x1929c, 0xaa0040L, 0xffff00ff,4 },
    { 0x1b04c, 0x20L, 0xfff0fff,4 },
    { 0x192a0, 0x80011dL, 0xff0fff,4 },
    { 0x19370, 0x4000L, 0x0,2 },
    { 0x192a4, 0x0L, 0x0,4 },
    { 0x1c0f4, 0x5000000L, 0x0,4 },
    { 0x192a8, 0x0L, 0x0,2 },
    { 0x1aa1c, 0x1L, 0x3,1 },
    { 0x1c078, 0x1L, 0x1,1 },
    { 0x18eac, 0x30L, 0x3f,1 },
    { 0x1aec0, 0x808b2L, 0xffffff,4 },
    { 0x1b02c, 0x6eL, 0xfff0fff,4 },
    { 0x18eb0, 0x2L, 0x1f,1 },
    { 0x1abc8, 0xf000L, 0xfffff,4 },
    { 0x1b2b4, 0x40L, 0xfff,2 },
    { 0x192b4, 0x0L, 0x17f,2 },
    { 0x1afe8, 0x1016L, 0x1fff,2 },
    { 0x1b12c, 0xf0L, 0xfff,2 },
    { 0x18eb8, 0x0L, 0x7,1 },
    { 0x19374, 0x8000L, 0x0,2 },
    { 0x1b278, 0x10L, 0xfff,2 },
    { 0x1c074, 0x0L, 0xfff,2 },
    { 0x18ebc, 0x0L, 0x7f,1 },
    { 0x1afdc, 0x21c03c0L, 0x0,4 },
    { 0x1b0dc, 0x0L, 0x0,1 },
    { 0x1c128, 0x0L, 0x0,4 },
    { 0x18ec0, 0xc5L, 0x0,1 },
    { 0x1aa20, 0x2L, 0x1f,1 },
    { 0x18ec4, 0x0L, 0xf,1 },
    { 0x1b14c, 0x1010L, 0x3f3f,2 },
    { 0x18ec8, 0x0L, 0x1f,1 },
    { 0x1abcc, 0xf000L, 0xfffff,4 },
    { 0x1c1e8, 0x0L, 0xfff,2 },
    { 0x18ecc, 0x0L, 0x301,2 },
    { 0x1c1e4, 0x0L, 0xfff0fff,4 },
    { 0x1b01c, 0x100L, 0xfff0fff,4 },
    { 0x18ed0, 0x1000000L, 0x1010226,4 },
    { 0x19378, 0xc000L, 0x0,2 },
    { 0x1b2a4, 0x0L, 0x3ff,2 },
    { 0x18ed4, 0x3010200L, 0x3030303,4 },
    { 0x1b11c, 0x0L, 0xfff,2 },
    { 0x18ed8, 0x0L, 0x1,1 },
    { 0x1aa24, 0x2L, 0xff0f,2 },
    { 0x1b164, 0xed8L, 0x0,2 },
    { 0x18edc, 0x3L, 0x0,1 },
    { 0x1ac28, 0x1L, 0x1,1 },
    { 0x18ee0, 0xfffffL, 0xfffff,4 },
    { 0x1abd0, 0xf000L, 0xfffff,4 },
    { 0x18ee4, 0xfffffL, 0xfffff,4 },
    { 0x1b124, 0xe7b4L, 0x0,2 },
    { 0x18ee8, 0xfffffL, 0xfffff,4 },
    { 0x1937c, 0x40000000L, 0x0,4 },
    { 0x1c090, 0x1180118L, 0x3fff3fff,4 },
    { 0x18eec, 0x8ffffL, 0xfffff,4 },
    { 0x1ac6c, 0x0L, 0x1,1 },
    { 0x18ef0, 0x8ffffL, 0xfffff,4 },
    { 0x1aa28, 0x7800L, 0xff07,2 },
    { 0x1b294, 0xfffaL, 0x0,2 },
    { 0x18ef4, 0x8ffffL, 0xfffff,4 },
    { 0x1b050, 0x0L, 0xfff,2 },
    { 0x1b0d0, 0x1L, 0x1,1 },
    { 0x18ef8, 0x0L, 0x0,2 },
    { 0x1aed4, 0x4000L, 0xfffff,4 },
    { 0x18efc, 0xfffffL, 0xfffff,4 },
    { 0x1b2c4, 0x2121L, 0x0,2 },
    { 0x1c100, 0x0L, 0x0,4 },
    { 0x1ae80, 0x5dL, 0x0,1 },
    { 0x1ac58, 0x1000L, 0x3ffff,4 },
    { 0x1b158, 0xed8L, 0x0,2 },
    { 0x1c144, 0x0L, 0x21f,2 },
    { 0x1ac30, 0x0L, 0xfffff,4 },
    { 0x18f08, 0x0L, 0x1,1 },
    { 0x1aa2c, 0x0L, 0x0,2 },
    { 0x1c080, 0xa004cL, 0xff00ff,4 },
    { 0x1aeb8, 0x1fa4L, 0x1fff,2 },
    { 0x18f0c, 0x3L, 0x0,1 },
    { 0x1c104, 0x0L, 0x0,4 },
    { 0x18f10, 0xfffffL, 0xfffff,4 },
    { 0x1aed8, 0xfa00000L, 0xfff0000,4 },
    { 0x1b27c, 0x20L, 0xfff,2 },
    { 0x18f14, 0xfffffL, 0xfffff,4 },
    { 0x1b0fc, 0x258L, 0xfff,2 },
    { 0x18f18, 0xfffffL, 0xfffff,4 },
    { 0x1ae84, 0x140064L, 0xfff0fff,4 },
    { 0x18f1c, 0x8ffffL, 0xfffff,4 },
    { 0x1c0f8, 0x0L, 0x0,4 },
    { 0x18f20, 0x8ffffL, 0xfffff,4 },
    { 0x1aa30, 0x0L, 0x0,2 },
    { 0x1c1ec, 0x1L, 0x1,1 },
    { 0x18f24, 0x8ffffL, 0xfffff,4 },
    { 0x1b030, 0xcdL, 0xfff,2 },
    { 0x18f28, 0x0L, 0x0,2 },
    { 0x1aedc, 0x4000L, 0xfffff,4 },
    { 0x1b2b8, 0xc7L, 0xfff,2 },
    { 0x18f2c, 0xfffffL, 0xfffff,4 },
    { 0x1afec, 0x101fL, 0x1fff,2 },
    { 0x1c260, 0x0L, 0x21f,2 },
    { 0x1b130, 0xe7b4L, 0x0,2 },
    { 0x1afc8, 0x1L, 0x1,1 },
    { 0x19288, 0x7d0L, 0x0,2 },
    { 0x1b134, 0x0L, 0xfff,2 },
    { 0x1b0ec, 0x800L, 0xfff,2 },
    { 0x1c064, 0x1L, 0x1,1 },
    { 0x1ac2c, 0x40807fL, 0xffffff,4 },
    { 0x18f38, 0x0L, 0x1,1 },
    { 0x1aa34, 0x0L, 0x0,2 },
    { 0x1b10c, 0xe7b4L, 0x0,2 },
    { 0x18f3c, 0x3L, 0x0,1 },
    { 0x18f40, 0xfffffL, 0xfffff,4 },
    { 0x1aee0, 0x4eL, 0xff0fff,4 },
    { 0x18f44, 0xfffffL, 0xfffff,4 },
    { 0x1aec4, 0xf0L, 0xfff0fff,4 },
    { 0x1b020, 0x2d0L, 0xfff,2 },
    { 0x18f48, 0xfffffL, 0xfffff,4 },
    { 0x1928c, 0x5L, 0xcd,1 },
    { 0x1b2a8, 0x1ffL, 0xfff,2 },
    { 0x18f4c, 0x8ffffL, 0xfffff,4 },
    { 0x1b054, 0x0L, 0x0,1 },
    { 0x1b120, 0xf0L, 0xfff,2 },
    { 0x18f50, 0x8ffffL, 0xfffff,4 },
    { 0x1aa38, 0x0L, 0x0,2 },
    { 0x18f54, 0x8ffffL, 0xfffff,4 },
    { 0x1c11c, 0xde78adf4L, 0x0,4 },
    { 0x18f58, 0x0L, 0x0,2 },
    { 0x1aee4, 0x4000L, 0xfffff,4 },
    { 0x18f5c, 0xfffffL, 0xfffff,4 },
    { 0x1b2d4, 0x0L, 0xfff,2 },
    { 0x1b730, 0x1L, 0x77,1 },
    { 0x19360, 0x0L, 0x0,4 },
    { 0x1ae90, 0x8000800L, 0xfff0fff,4 },
    { 0x1c094, 0x0L, 0xf,1 },
    { 0x19364, 0xffffffffL, 0x0,4 },
    { 0x1c1d8, 0x1L, 0x1,1 },
    { 0x1c108, 0x0L, 0xf,1 },
    { 0x18f68, 0x0L, 0x1,1 },
    { 0x1aa3c, 0x40L, 0x0,4 },
    { 0x1ae88, 0xf00064L, 0xfff0fff,4 },
    { 0x18f6c, 0x3L, 0x0,1 },
    { 0x1b298, 0x2121L, 0x0,2 },
    { 0x1ac50, 0x400L, 0x0,2 },
    { 0x18f70, 0xfffffL, 0xfffff,4 },
    { 0x1aee8, 0xfa0L, 0xfff,2 },
    { 0x18f74, 0xfffffL, 0xfffff,4 },
    { 0x1b154, 0x100L, 0xfff,2 },
    { 0x1c10c, 0x1e00L, 0x0,4 },
    { 0x18f78, 0xfffffL, 0xfffff,4 },
    { 0x1ae94, 0x800L, 0xfff0fff,4 },
    { 0x1b0f8, 0x0L, 0xfff,2 },
    { 0x18f7c, 0x8ffffL, 0xfffff,4 },
    { 0x1ac48, 0xffff0000L, 0x0,4 },
    { 0x1ac34, 0xd9999L, 0xfffff,4 },
    { 0x18f80, 0x8ffffL, 0xfffff,4 },
    { 0x1b040, 0xd48L, 0xfff,2 },
    { 0x1c084, 0x12cL, 0xff03ff,4 },
    { 0x1aebc, 0x1L, 0x1,1 },
    { 0x18f84, 0x8ffffL, 0xfffff,4 },
    { 0x1b000, 0x171L, 0x1fff,2 },
    { 0x18f88, 0x0L, 0x0,2 },
    { 0x1aeec, 0x4000L, 0xfffff,4 },
    { 0x1b288, 0x0L, 0x0,2 },
    { 0x18f8c, 0xfffffL, 0xfffff,4 },
    { 0x1afd4, 0x200fffL, 0xff0fff,4 },
    { 0x1ac40, 0x200L, 0x3ff,2 },
    { 0x1ae98, 0x0L, 0xfff0fff,4 },
    { 0x1b110, 0x0L, 0xfff,2 },
    { 0x1c0fc, 0x0L, 0x0,4 },
    { 0x1b2c8, 0xfffL, 0xfff,2 },
    { 0x18f98, 0x20000L, 0x70007,4 },
    { 0x1b044, 0x5dcL, 0xfff0fff,4 },
    { 0x18f9c, 0x0L, 0x3ffff,4 },
    { 0x1b034, 0x46L, 0xfff0fff,4 },
    { 0x18fa0, 0x0L, 0x1fff,2 },
    { 0x1aff0, 0x159L, 0x1fff,2 },
    { 0x1aeac, 0x0L, 0x0,1 },
    { 0x18fa4, 0x0L, 0xfff,2 },
    { 0x18fa8, 0x0L, 0x0,4 },
    { 0x1ae9c, 0x11d1cL, 0xffffff,4 },
    { 0x1c2b8, 0x0L, 0x21f,2 },
    { 0x18fac, 0x0L, 0x0,2 },
    { 0x1b0f0, 0xcccL, 0xfff,2 },
    { 0x18fb0, 0x0L, 0xf0f0f0f,4 },
    { 0x19348, 0x5cL, 0x0,1 },
    { 0x1b15c, 0x0L, 0xfff,2 },
    { 0x1b140, 0x0L, 0xfff,2 },
    { 0x18fb4, 0x0L, 0xfff0fff,4 },
    { 0x1c0ec, 0x202L, 0xbdf,2 },
    { 0x18fb8, 0x0L, 0xfff0fff,4 },
    { 0x1aff4, 0x103aL, 0x1fff,2 },
    { 0x18fbc, 0x0L, 0xfff0fff,4 },
    { 0x1c1dc, 0x1000100L, 0xfff0fff,4 },
    { 0x1b024, 0xfff01f4L, 0xfff0fff,4 },
    { 0x18fc0, 0x0L, 0xfff0fff,4 },
    { 0x1aea0, 0x0L, 0x0,1 },
    { 0x1b2ac, 0x40L, 0xfff,2 },
    { 0x18fc4, 0x0L, 0xfff0fff,4 },
    { 0x1afe0, 0x1bbL, 0x1fff,2 },
    { 0x1b0e4, 0x3fL, 0x7f,1 },
    { 0x1b284, 0xe0L, 0xfff,2 },
    { 0x18fc8, 0x0L, 0xfff0fff,4 },
    { 0x1934c, 0x3aL, 0x3f,1 },
    { 0x1c068, 0x1000100L, 0xfff0fff,4 },
    { 0x1b764, 0x2121L, 0x0,2 },
    { 0x18fcc, 0x0L, 0xfff0fff,4 },
    { 0x1b0e0, 0x96L, 0xfff,2 },
    { 0x18fd0, 0x0L, 0xfff0fff,4 },
    { 0x1aff8, 0x15L, 0x1fff,2 },
    { 0x1abd4, 0xf000L, 0xfffff,4 },
    { 0x1b29c, 0x0L, 0x1,1 },
    { 0x1afd8, 0xe95L, 0x0,2 },
    { 0x1aea4, 0xcf01b3L, 0xff0fff,4 },
    { 0x1aed0, 0x8L, 0xff0fff,4 },
    { 0x1aec8, 0x157c8000L, 0x0,4 },
    { 0x1afd0, 0x40000c8L, 0xfff0fff,4 },
    { 0x19350, 0x8L, 0x0,1 },
    { 0x1ae8c, 0x960171L, 0xfff0fff,4 },
    { 0x18fe4, 0x2L, 0xff03,2 },
    { 0x1c06c, 0x100L, 0xfff,2 },
    { 0x18fe8, 0x1000100L, 0xfff0fff,4 },
    { 0x1affc, 0x1086L, 0x1fff,2 },
    { 0x1b150, 0x0L, 0xfff,2 },
    { 0x1b114, 0xf0L, 0xfff,2 },
    { 0x18fec, 0x100L, 0xfff,2 },
    { 0x1c110, 0x1L, 0xf,1 },
    { 0x18ff0, 0xc000f00L, 0xfff0fff,4 },
    { 0x1aea8, 0x5596L, 0xffffff,4 },
    { 0x18ff4, 0xc000f00L, 0xfff0fff,4 },
    { 0x1ac38, 0x100L, 0xfff,2 },
    { 0x18ff8, 0xc000f00L, 0xfff0fff,4 },
    { 0x19354, 0x21c03c0L, 0x0,4 },
    { 0x1c088, 0x3e803e8L, 0x3ffffff,4 },
    { 0x18ffc, 0xf000f0L, 0xfff0fff,4 },
    { 0x1b2c0, 0x1b29acL, 0x0,4 },
    { 0x1ae7c, 0xa0b9beb9L, 0xffffffff,4 },
    { 0x1ae84, 0x5aL, 0xff,1 },
    { 0x1ae84, 0x32L, 0xff,1 },
    { 0x1ae84, 0x50L, 0xff,1 },
    { 0x1ae84, 0x5aL, 0xff,1 },
    { 0x1aea8, 0x5fa0L, 0xffff,2 },
    { 0x80L,   0x1L,  0xff,1 },
    { 0x19284, 0x4000L, 0xffff,2 },
    { 0x19288, 0x400L, 0xffff,2 },
    { 0x1abc0, 0x400L, 0xffff,2 },
    { 0x1abc4, 0x4000L, 0xffff,2 },
    //stop sequence - address is 0x0000
    { 0x0000, 0x0000, 0x0000, 0x0000 }
};

static const acam_reg_t *seq_table[] = {
    init,
	setting_2200_2720_2lane_1g_30fps,
	setting_2200_2720_2lane_1g_30fps_test_pattern,
	setting_2200_2720_2lane_960m_30fps_test_pattern,
	setting_1920_1080_2lane_1g_30fps_test_pattern,
	setting_1920_1080_2lane_1g_30fps,
};

static const acam_reg_t *isp_seq_table[] = {
	settings_context_imx227,
};


#define SENSOR_IMX227_SEQUENCE_DEFAULT seq_table
#define SENSOR_IMX227_ISP_CONTEXT_SEQUENCE isp_seq_table


#define SENSOR_IMX227_SEQUENCE_DEFAULT_INIT    0
#define SENSOR_IMX227_SEQUENCE_DEFAULT_PREVIEW    1
#define SENSOR_IMX227_SEQUENCE_TESTPATTERN    2
#define SENSOR_IMX227_SEQUENCE_TESTPATTERN_960M   3
#define SENSOR_IMX227_SEQUENCE_1080P_TESTPATTERN  4
#define SENSOR_IMX227_SEQUENCE_1080P_PREVIEW  5
#define SENSOR_IMX227_CONTEXT_SEQ  0

#endif // __IMX227_SEQ_H__