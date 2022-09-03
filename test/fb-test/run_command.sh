#!/bin/bash

# python ../run_tests.py --working_dir fb-test --test_spec fb_test.json --for_flatbuffers --include_flatbuffers
# python ../run_tests.py --working_dir . --test_spec ../fb_test.json --for_flatbuffers --include_flatbuffers --ignore_dirty
python ../run_tests.py --working_dir . --test_spec ../fb_test.json --for_flatbuffers --ignore_dirty