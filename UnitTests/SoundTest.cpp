//
//  SoundTest.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/01/16.
//
//

#include <gtest/gtest.h>

#include "AudioFactory.h"

TEST(Sound, LoadWaveAndExpectSuccess)
{
    const mono::SoundFile& file = mono::AudioFactory::LoadFile("a.wav");

    EXPECT_EQ(file.format, mono::SoundFormat::MONO_8BIT);
    EXPECT_EQ(file.frequency, 11025);
    EXPECT_EQ(file.data.size(), 1793);
}

TEST(Sound, LoadMissingFileExpectException)
{
    EXPECT_THROW(mono::AudioFactory::LoadFile("whatever.asdf"), std::runtime_error);
}