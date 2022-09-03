// Copyright (c) by respective owners including Yahoo!, Microsoft, and
// individual contributors. All rights reserved. Released under a BSD (revised)
// license as described in the file LICENSE.

#include "vw/core/constant.h"
#include "vw/core/feature_group.h"
#include "vw/core/parse_example.h"
#include "vw/core/vw.h"
#include "vw/fb_parser/parse_example_flatbuffer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>


#include "../../utl/flatbuffer/vw_to_flat.h"
// #include "vw_to_flat.h"
#ifdef _WIN32
#  define NOMINMAX
#  include <WinSock2.h>
#else
#  include <arpa/inet.h>
#  include <sys/socket.h>
#endif
#include "vw/common/vw_exception.h"
#include "vw/config/options.h"
#include "vw/config/options_cli.h"
#include "vw/core/accumulate.h"
#include "vw/core/best_constant.h"
#include "vw/core/parse_args.h"
#include "vw/core/parse_regressor.h"


// #include "vw/core/vw.h"
// VW_WARNING_STATE_PUSH
// VW_WARNING_DISABLE_BADLY_FORMED_XML
// #include "vw/fb_parser/generated/example_generated.h"
// VW_WARNING_STATE_POP
// #include "vw/core/named_labels.h"
// #include "vw/core/simple_label.h"

// #include "vw/core/vw.h"
#include <sys/timeb.h>
#include <fstream>
#include <iostream>
#include <string>


flatbuffers::Offset<void> get_label(flatbuffers::FlatBufferBuilder& builder, VW::parsers::flatbuffer::Label label_type)
{
  flatbuffers::Offset<void> label;
  if (label_type == VW::parsers::flatbuffer::Label_SimpleLabel)
    label = VW::parsers::flatbuffer::CreateSimpleLabel(builder, 0.0, 1.0).Union();

  return label;
}

flatbuffers::Offset<VW::parsers::flatbuffer::ExampleRoot> sample_flatbuffer_collection(
    flatbuffers::FlatBufferBuilder& builder, VW::parsers::flatbuffer::Label label_type)
{
  std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Example>> examples;
  std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Namespace>> namespaces;
  // std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Feature>> fts;

  auto label = get_label(builder, label_type);

  // fts.push_back(VW::parsers::flatbuffer::CreateFeatureDirect(builder, "hello", 2.23f, constant));
  std::vector<flatbuffers::Offset<flatbuffers::String>> feature_names = {builder.CreateString("hello")};
  std::vector<float> feature_values = {2.23f};
  std::vector<uint64_t> feature_hashes = {constant};
  namespaces.push_back(VW::parsers::flatbuffer::CreateNamespaceDirect(builder, nullptr, constant_namespace, 128, &feature_names, &feature_values, &feature_hashes));
  // namespaces.push_back(VW::parsers::flatbuffer::CreateNamespaceDirect(builder, nullptr, 128, constant_namespace, "hello", 2.23f, constant, 128));
  examples.push_back(VW::parsers::flatbuffer::CreateExampleDirect(builder, &namespaces, label_type, label));

  auto eg_collection = VW::parsers::flatbuffer::CreateExampleCollectionDirect(builder, &examples);
  return CreateExampleRoot(builder, VW::parsers::flatbuffer::ExampleType_ExampleCollection, eg_collection.Union());
}

flatbuffers::Offset<VW::parsers::flatbuffer::ExampleRoot> sample_flatbuffer(
    flatbuffers::FlatBufferBuilder& builder, VW::parsers::flatbuffer::Label label_type)
{
  std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Namespace>> namespaces;
  // std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Feature>> fts;
  auto label = get_label(builder, label_type);

  // fts.push_back(VW::parsers::flatbuffer::CreateFeatureDirect(builder, "hello", 2.23f, constant));
   const std::vector<flatbuffers::Offset<flatbuffers::String>> feature_names = {builder.CreateString("hello")}; //auto temp_fn= {builder.CreateString("hello")}; 
   const std::vector<float>  feature_values = {2.23f};
   const std::vector<uint64_t> feature_hashes = {constant}; 
  namespaces.push_back(VW::parsers::flatbuffer::CreateNamespaceDirect(builder, nullptr, constant_namespace, 128, &feature_names, &feature_values, &feature_hashes));
  auto example = VW::parsers::flatbuffer::CreateExampleDirect(builder, &namespaces, label_type, label);

  return CreateExampleRoot(builder, VW::parsers::flatbuffer::ExampleType_Example, example.Union());
}

flatbuffers::Offset<VW::parsers::flatbuffer::ExampleRoot> sample_flatbuffer_features_collection(
    flatbuffers::FlatBufferBuilder& builder, VW::parsers::flatbuffer::Label label_type)
{
  std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Example>> examples;
  std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Namespace>> namespaces;
  // std::vector<flatbuffers::Offset<VW::parsers::flatbuffer::Feature>> fts;

  auto label = get_label(builder, label_type);

  std::vector<flatbuffers::Offset<flatbuffers::String>> feature_names = {builder.CreateString("hello"), builder.CreateString("world")};
  // flatbuffers::Offset<flatbuffers::String> str2 = 0;
  // flatbuffers::Offset<flatbuffers::String> str2 = builder.CreateString("world");
  // std::vector<flatbuffers::Offset<flatbuffers::String>> feature_names = {builder.CreateString("hello"), str2};
  std::vector<float> feature_values = {2.23f, 5.0f};
  std::vector<uint64_t> feature_hashes = {constant, constant};
  namespaces.push_back(VW::parsers::flatbuffer::CreateNamespaceDirect(builder, nullptr, constant_namespace, 128, &feature_names, &feature_values, &feature_hashes));
  // namespaces.push_back(VW::parsers::flatbuffer::CreateNamespaceDirect(builder, nullptr, 128, constant_namespace, "hello", 2.23f, constant, 128));
  examples.push_back(VW::parsers::flatbuffer::CreateExampleDirect(builder, &namespaces, label_type, label));

  auto eg_collection = VW::parsers::flatbuffer::CreateExampleCollectionDirect(builder, &examples);
  return CreateExampleRoot(builder, VW::parsers::flatbuffer::ExampleType_ExampleCollection, eg_collection.Union());
}


TEST(flatbuffer_parser_tests, test_flatbuffer_standalone_example)
{
  auto all = VW::initialize("--no_stdin --quiet --flatbuffer", nullptr, false, nullptr, nullptr);

  flatbuffers::FlatBufferBuilder builder;

  auto root = sample_flatbuffer(builder, VW::parsers::flatbuffer::Label_SimpleLabel);
  builder.FinishSizePrefixed(root);

  uint8_t* buf = builder.GetBufferPointer();
  int size = builder.GetSize();

  VW::multi_ex examples;
  examples.push_back(&VW::get_unused_example(all));
  io_buf unused_buffer;
  all->flat_converter->parse_examples(all, unused_buffer, examples, buf);

  auto example = all->flat_converter->data()->example_obj_as_Example();
  EXPECT_EQ(example->namespaces()->size(), 1);
  EXPECT_EQ(example->namespaces()->Get(0)->feature_names()->size(), 1);
  EXPECT_FLOAT_EQ(example->label_as_SimpleLabel()->label(), 0.0);
  EXPECT_FLOAT_EQ(example->label_as_SimpleLabel()->weight(), 1.0);
  EXPECT_EQ(example->namespaces()->Get(0)->hash(), constant_namespace);
  EXPECT_EQ(example->namespaces()->Get(0)->full_hash(), constant_namespace);
  EXPECT_STREQ(example->namespaces()->Get(0)->feature_names()->Get(0)->c_str(), "hello");
  EXPECT_EQ(example->namespaces()->Get(0)->feature_hashes()->Get(0), constant);
  EXPECT_FLOAT_EQ(example->namespaces()->Get(0)->feature_values()->Get(0), 2.23);

  // Check vw example
  EXPECT_EQ(examples.size(), 1);
  EXPECT_FLOAT_EQ(examples[0]->l.simple.label, 0.f);
  const auto& red_features = examples[0]->_reduction_features.template get<simple_label_reduction_features>();
  EXPECT_FLOAT_EQ(red_features.weight, 1.f);

  EXPECT_EQ(examples[0]->indices[0], constant_namespace);
  EXPECT_FLOAT_EQ(examples[0]->feature_space[examples[0]->indices[0]].values[0], 2.23f);
  EXPECT_EQ(examples[0]->feature_space[examples[0]->indices[0]].namespace_extents.size(), 1);
  EXPECT_EQ(examples[0]->feature_space[examples[0]->indices[0]].namespace_extents[0],
      (VW::namespace_extent{0, 1, constant_namespace}));

  VW::finish_example(*all, *examples[0]);
  VW::finish(*all);
}

TEST(flatbuffer_parser_tests, test_flatbuffer_collection)
{
  auto all = VW::initialize("--no_stdin --quiet --flatbuffer", nullptr, false, nullptr, nullptr);

  flatbuffers::FlatBufferBuilder builder;

  auto root = sample_flatbuffer_collection(builder, VW::parsers::flatbuffer::Label_SimpleLabel);
  builder.FinishSizePrefixed(root);

  uint8_t* buf = builder.GetBufferPointer();
  int size = builder.GetSize();

  VW::multi_ex examples;
  examples.push_back(&VW::get_unused_example(all));
  io_buf unused_buffer;
  all->flat_converter->parse_examples(all, unused_buffer, examples, buf);

  auto collection_examples = all->flat_converter->data()->example_obj_as_ExampleCollection()->examples();
  EXPECT_EQ(collection_examples->size(), 1);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->size(), 1);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_names()->size(), 1);
  EXPECT_FLOAT_EQ(collection_examples->Get(0)->label_as_SimpleLabel()->label(), 0.0);
  EXPECT_FLOAT_EQ(collection_examples->Get(0)->label_as_SimpleLabel()->weight(), 1.0);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->hash(), constant_namespace);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->full_hash(), constant_namespace);
  EXPECT_STREQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_names()->Get(0)->c_str(), "hello");
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_hashes()->Get(0), constant);
  EXPECT_FLOAT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_values()->Get(0), 2.23);

  // check vw example
  EXPECT_EQ(examples.size(), 1);
  EXPECT_FLOAT_EQ(examples[0]->l.simple.label, 0.f);
  const auto& red_features = examples[0]->_reduction_features.template get<simple_label_reduction_features>();
  EXPECT_FLOAT_EQ(red_features.weight, 1.f);

  EXPECT_EQ(examples[0]->indices[0], constant_namespace);
  EXPECT_FLOAT_EQ(examples[0]->feature_space[examples[0]->indices[0]].values[0], 2.23f);
  EXPECT_EQ(examples[0]->feature_space[examples[0]->indices[0]].namespace_extents.size(), 1);
  EXPECT_EQ(examples[0]->feature_space[examples[0]->indices[0]].namespace_extents[0],
      (VW::namespace_extent{0, 1, constant_namespace}));

  VW::finish_example(*all, *examples[0]);
  VW::finish(*all);
}


TEST(flatbuffer_parser_tests, test_flatbuffer_features_collection)
{
  auto all = VW::initialize("--no_stdin --quiet --flatbuffer", nullptr, false, nullptr, nullptr);

  flatbuffers::FlatBufferBuilder builder;

  auto root = sample_flatbuffer_features_collection(builder, VW::parsers::flatbuffer::Label_SimpleLabel);
  builder.FinishSizePrefixed(root);

  uint8_t* buf = builder.GetBufferPointer();
  int size = builder.GetSize();

  VW::multi_ex examples;
  examples.push_back(&VW::get_unused_example(all));
  io_buf unused_buffer;
  all->flat_converter->parse_examples(all, unused_buffer, examples, buf);

  auto collection_examples = all->flat_converter->data()->example_obj_as_ExampleCollection()->examples();
  EXPECT_EQ(collection_examples->size(), 1);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->size(), 1);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_names()->size(), 2);
  EXPECT_FLOAT_EQ(collection_examples->Get(0)->label_as_SimpleLabel()->label(), 0.0);
  EXPECT_FLOAT_EQ(collection_examples->Get(0)->label_as_SimpleLabel()->weight(), 1.0);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->hash(), constant_namespace);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->full_hash(), constant_namespace);
  EXPECT_STREQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_names()->Get(0)->c_str(), "hello");
  EXPECT_STREQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_names()->Get(1)->c_str(), "world");
  // EXPECT_STREQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_names()->Get(1)->c_str(), nullptr);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_hashes()->Get(0), constant);
  EXPECT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_hashes()->Get(1), constant);
  EXPECT_FLOAT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_values()->Get(0), 2.23);
  EXPECT_FLOAT_EQ(collection_examples->Get(0)->namespaces()->Get(0)->feature_values()->Get(1), 5.0);

  // check vw example
  EXPECT_EQ(examples.size(), 1);
  EXPECT_FLOAT_EQ(examples[0]->l.simple.label, 0.f);
  const auto& red_features = examples[0]->_reduction_features.template get<simple_label_reduction_features>();
  EXPECT_FLOAT_EQ(red_features.weight, 1.f);

  EXPECT_EQ(examples[0]->indices[0], constant_namespace);
  EXPECT_FLOAT_EQ(examples[0]->feature_space[examples[0]->indices[0]].values[0], 2.23f);
  EXPECT_EQ(examples[0]->feature_space[examples[0]->indices[0]].namespace_extents.size(), 1);
  EXPECT_EQ(examples[0]->feature_space[examples[0]->indices[0]].namespace_extents[0],
      (VW::namespace_extent{0, 2, constant_namespace}));

  VW::finish_example(*all, *examples[0]);
  VW::finish(*all);
}

// using namespace VW::config;

// VW::workspace* setup(std::unique_ptr<options_i, options_deleter_type> options)
// {
//   VW::workspace* all = nullptr;
//   try
//   {
//     all = VW::initialize(std::move(options));
//   }
//   catch (const std::exception& ex)
//   {
//     std::cout << ex.what() << std::endl;
//     throw;
//   }
//   catch (...)
//   {
//     std::cout << "unknown exception" << std::endl;
//     throw;
//   }
//   all->vw_is_main = true;

//   if (!all->quiet && !all->bfgs && !all->searchstr && !all->options->was_supplied("audit_regressor"))
//   { all->sd->print_update_header(*(all->trace_message)); }

//   return all;
// }

// TEST(flatbuffer_parser_tests, measure_file_size)
// {
// using namespace VW::config;

// auto vw = VW::initialize("--no_stdin --quiet --flatbuffer", nullptr, false, nullptr, nullptr);
// //   auto vw = VW::initialize(command_line, nullptr, false, nullptr, nullptr);
//   std::vector<example*> examples;
//   examples.push_back(VW::read_example(*vw,
//       std::string(
//           "-1 |f 5:3.4770757e-02 21:5.2058056e-02 22:1.0131893e-01 66:9.8602206e-02 126:7.3677950e-02 "
//           "144:8.3734207e-02 "
//           "258:1.3117002e-01 282:1.0712786e-01 286:8.9001723e-02 302:6.8243802e-02 454:1.1229499e-01 459:8.5936494e-02 "
//           "533:4.6158761e-02 534:1.0838871e-01 537:2.2317035e-01 540:1.0823997e-01 550:7.0598722e-02 573:6.0984179e-02 "
//           "617:6.9755495e-02 620:5.3597409e-02 809:1.2329303e-01 811:1.0078616e-01 866:1.6002567e-01 "
//           "1071:6.7204006e-02 "
//           "1134:8.8398121e-02 1305:8.9227840e-02 1314:1.5046395e-01 1383:8.7426804e-02 2298:3.2898754e-02 "
//           "2477:1.0394224e-01 "
//           "3000:2.1763121e-01 3793:1.5177026e-01 5225:1.4700839e-01 7108:2.1639362e-01 7165:1.5993990e-01 "
//           "7275:1.5572040e-01 "
//           "7851:2.7698877e-01 7853:2.0813602e-01 7854:4.2525813e-01 7876:1.9971584e-01 7922:2.3383786e-01 "
//           "7928:2.2501056e-01 "
//           "22893:2.4975674e-01")));

//   examples.push_back(VW::read_example(*vw,
//       std::string(
//           "1 |f 24:2.5941234e-02 45:1.0093741e-01 73:4.4377144e-02 142:1.1601033e-01 159:1.9448641e-01 "
//           "197:2.5329438e-01 "
//           "257:6.3567974e-02 488:1.6744833e-01 497:1.3232227e-01 547:1.2052315e-01 613:5.2947123e-02 759:2.0554376e-01 "
//           "782:1.9669981e-01 905:1.0293012e-01 1211:1.1731350e-01 1383:9.5941707e-02 1398:1.7192762e-01 "
//           "1479:1.2192473e-01 "
//           "1590:2.0290023e-01 1765:1.7032301e-01 2493:1.7820080e-01 3132:1.4529058e-01 3271:2.2052087e-01 "
//           "3405:1.6775523e-01 "
//           "5317:1.7017807e-01 6556:2.1031970e-01 8228:2.1553637e-01 12077:4.8640794e-01 28872:2.9970971e-01")));


//     flatbuffers::FlatBufferBuilder builder;

//     auto root = sample_flatbuffer_features_collection(builder, VW::parsers::flatbuffer::Label_SimpleLabel);
//     builder.FinishSizePrefixed(root);

//     uint8_t* buf = builder.GetBufferPointer();
//     int size = builder.GetSize();

//     // VW::multi_ex examples = benchmark_rcv1_dataset_get_examples(&all);
//     examples.push_back(&VW::get_unused_example(vw));
//     io_buf unused_buffer;
//     vw->flat_converter->parse_examples(vw, unused_buffer, examples, buf);

    
//     option_group_definition driver_config("Driver");
//     to_flat converter;
//     driver_config.add(make_option("fb_out", converter.output_flatbuffer_name));
//     driver_config.add(make_option("collection_size", converter.collection_size));

//     // std::vector<VW::workspace*> alls;
//     // std::vector<std::string> opts(argv + 1, argv + argc);
//     // opts.emplace_back("--quiet");

//     std::vector<VW::workspace*> alls;
//     std::vector<std::string> opts;
//     opts.emplace_back("--no_stdin --quiet --flatbuffer");
//     std::unique_ptr<options_cli, options_deleter_type> ptr(
//         new options_cli(opts), [](VW::config::options_i* ptr) { delete ptr; });
//     ptr->add_and_parse(driver_config);
//     alls.push_back(setup(std::move(ptr)));
//     if (converter.collection_size > 0) { converter.collection = true; }

//     // // std::unique_ptr<options_cli, options_deleter_type> ptr(
//     // //     new options_cli(opts), [](VW::config::options_i* ptr) { delete ptr; });
//     // vw->add_and_parse(driver_config);
//     // // vw.push_back(setup(std::move(ptr)));
//     // if (converter.collection_size > 0) { converter.collection = true; }

//     VW::workspace& all = *alls[0];

//     VW::start_parser(all);
//     converter.convert_txt_to_flat(all);
//     VW::end_parser(all);
//     // return 0;
// }

