#define MCAP_IMPLEMENTATION
#include "include/mcap/writer.hpp"
#include "include/BuildFileDescriptorSet.h"
#include "data.pb.h" // 包含由protoc生成的头文件

int main()
{
  // 创建一个MCAP writer对象，指定输出文件的路径。
  const char* outputFilename = "aaa.mcap";
  mcap::McapWriter writer;
  {
    auto options = mcap::McapWriterOptions("");
    const auto res = writer.open(outputFilename, options);
    if (!res.ok())
    {
      std::cerr << "Failed to open " << outputFilename << " for writing: " << res.message
                << std::endl;
      return 1;
    }
  }
  mcap::ChannelId channelId;
  {
    mcap::Schema schema(
      "Data", "protobuf",
      foxglove::BuildFileDescriptorSet(Data::descriptor()).SerializeAsString());
    writer.addSchema(schema);

    // choose an arbitrary topic name.
    mcap::Channel channel("Data", "protobuf", schema.id);
    writer.addChannel(channel);
    channelId = channel.id;
  }

  // 创建一个ExampleMessage实例并填充数据。
  Data message;
  Head head;
  head.set_id(1);
  head.set_index(3);
  head.set_timestamp(2);

  message.set_allocated_head(&head);
  message.set_speed(80);
  message.set_acc(5);

  mcap::Timestamp startTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
                                std::chrono::system_clock::now().time_since_epoch())
                                .count();
  for (uint32_t frameIndex = 0; frameIndex < 100; ++frameIndex) {
    mcap::Timestamp cloudTime = startTime + (static_cast<uint64_t>(frameIndex) * 100 * 1000000);
    head.set_index(head.index() + 1);
    message.set_speed(message.speed() + 1);
    message.set_acc(message.acc() + 1);
    std::string serialized = message.SerializeAsString();
    mcap::Message msg;
    msg.channelId = channelId;
    msg.sequence = frameIndex;
    msg.publishTime = cloudTime;
    msg.logTime = cloudTime;
    msg.data = reinterpret_cast<const std::byte*>(serialized.data());
    msg.dataSize = serialized.size();
    const auto res = writer.write(msg);
    if (!res.ok()) {
      std::cerr << "Failed to write message: " << res.message << "\n";
      writer.terminate();
      writer.close();
      std::ignore = std::remove(outputFilename);
      return 1;
    }
  }

  
  // Write the index and footer to the file, then close it.
  writer.close();
  std::cerr << "wrote 100 pointcloud messages to " << outputFilename << std::endl;
  std::cerr << "end!!! " << std::endl;
  return 0;
}