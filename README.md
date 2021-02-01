# simple-json-lib

supports types: std::string, int, float

requires C++17 or above

# Using the library
examples in main.cpp

loading file:

```std::map<first type, second type> example = json::load<second type>(file path pointer);```


dumping into file:

```json::dump(file path pointer, map pointer);```
