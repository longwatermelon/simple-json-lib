# simple-json-lib

supports types: std::string, int, float, std::vector

requires C++17 or above

# Using the library
examples in main.cpp

loading file:

```std::map<first type, second type> example = json::load<second type>(file path pointer);```

or if you dont like angular brackets

```json::load(file path pointer, map pointer);```


dumping into file:

```json::dump(file path pointer, map pointer);```
