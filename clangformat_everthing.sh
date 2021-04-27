#!/bin/bash
find ./Source | grep "\.[ch]pp" | xargs clang-format -i
