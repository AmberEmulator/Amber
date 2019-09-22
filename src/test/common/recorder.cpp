#include <catch2/catch.hpp>

#include <common/recorder.hpp>

using namespace Amber;
using namespace Common;

TEST_CASE("Recorder can save and restore frame data")
{
	struct FooData
	{
		size_t m_Field0;
		size_t m_Field1;
	};

	struct BarData
	{
		uint8_t m_Field0[2];
		uint16_t m_Field1;
		uint8_t m_Field3[17];
	};

	ChannelDescription foo_channel_description;
	foo_channel_description.SetName("Foo");
	foo_channel_description.SetSize(sizeof(FooData));

	ChannelDescription bar_channel_description;
	bar_channel_description.SetName("Bar");
	bar_channel_description.SetSize(sizeof(BarData));

	RecorderDescription recorder_description;
	const size_t foo_channel = recorder_description.AddChannel(foo_channel_description);
	REQUIRE(foo_channel == 0);
	REQUIRE(recorder_description.GetChannel(foo_channel).GetName() == foo_channel_description.GetName());
	REQUIRE(recorder_description.GetChannel(foo_channel).GetSize() == foo_channel_description.GetSize());
	REQUIRE(recorder_description.GetChannelCount() == 1);
	REQUIRE(recorder_description.FindChannel(foo_channel_description.GetName()).value() == foo_channel);

	const size_t bar_channel = recorder_description.AddChannel(bar_channel_description);
	REQUIRE(bar_channel == 1);
	REQUIRE(recorder_description.GetChannel(bar_channel).GetName() == bar_channel_description.GetName());
	REQUIRE(recorder_description.GetChannel(bar_channel).GetSize() == bar_channel_description.GetSize());
	REQUIRE(recorder_description.GetChannelCount() == 2);
	REQUIRE(recorder_description.FindChannel(foo_channel_description.GetName()).value() == foo_channel);
	REQUIRE(recorder_description.FindChannel(bar_channel_description.GetName()).value() == bar_channel);

	Recorder recorder(recorder_description);

	constexpr size_t total_frame_count = 100000;

	for (size_t i = 0; i < total_frame_count; ++i)
	{
		const size_t new_frame = recorder.NewFrame();
		REQUIRE(new_frame == i);
		REQUIRE(recorder.GetCurrentFrame() == new_frame);

		// Write FooData every frame
		FooData foo_data;
		foo_data.m_Field0 = i;
		foo_data.m_Field1 = i / 100;
		recorder.WriteChannelData(foo_channel, &foo_data);

		REQUIRE(std::memcmp(&foo_data, recorder.ReadChannelData(foo_channel), sizeof(foo_data)) == 0);

		// Write BarData every few frames
		if (i % 7 == 0)
		{
			BarData bar_data = {};
			bar_data.m_Field0;
			bar_data.m_Field1 = i;
			recorder.WriteChannelData(bar_channel, &bar_data);

			REQUIRE(std::memcmp(&bar_data, recorder.ReadChannelData(bar_channel), sizeof(bar_data)) == 0);
		}
	}

	REQUIRE(recorder.GetFrameCount() == total_frame_count);

	for (size_t i = 0; i < total_frame_count; --i)
	{
		const size_t frame_index = total_frame_count - (i + 1);

		recorder.SetCurrentFrame(frame_index);
		REQUIRE(recorder.GetCurrentFrame() == frame_index);

		FooData expected_foo_data;
		expected_foo_data.m_Field0 = frame_index;
		expected_foo_data.m_Field1 = frame_index / 100;

		FooData foo_data;
		std::memcpy(&foo_data, recorder.ReadChannelData(foo_channel), sizeof(FooData));

		REQUIRE(foo_data.m_Field0 == expected_foo_data.m_Field0);
		REQUIRE(foo_data.m_Field1 == expected_foo_data.m_Field1);

		BarData expected_bar_data = {};
		expected_bar_data.m_Field0;
		expected_bar_data.m_Field1 = (frame_index / 7) * 7;

		BarData bar_data;
		std::memcpy(&bar_data, recorder.ReadChannelData(bar_channel), sizeof(FooData));

		REQUIRE(bar_data.m_Field0[0] == expected_bar_data.m_Field0[0]);
		REQUIRE(bar_data.m_Field0[1] == expected_bar_data.m_Field0[1]);
		REQUIRE(bar_data.m_Field1 == expected_bar_data.m_Field1);
	}
}