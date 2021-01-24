# simple-json-lib

supports types: std::string, int, float

requires C++17 or above

# Using the library
loading file:

```std::map<first type, second type> example = json::load<first type, second type>(filepath);```


dumping into file:

```json::dump<first type, second type>(file path, map name);```
