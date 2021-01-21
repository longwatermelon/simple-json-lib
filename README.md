# simple-json-lib

work in progress

only supports std::string and int for now

# Using the library
loading file:

```std::map<first type, second type> example = json::load<first type, second type>(filepath);```


dumping into file:

```json::dump<first type, second type>(file path, map name);```
