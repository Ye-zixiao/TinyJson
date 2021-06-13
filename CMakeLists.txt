cmake_minimum_required(VERSION 3.16)
project(Json)

set(CMAKE_CXX_STANDARD 17)

add_executable(Json main.cpp json/JsonValue.h json/JsonValue.cpp json/Handler.h json/ParseException.h json/StringReadStream.h json/noncopyable.h json/StringWriteStream.h json/JsonReader.h json/FileReadStream.h json/FileWriteStream.h json/JsonReader.cpp json/Document.h json/JsonBaseWriter.h json/JsonSample.h json/JsonPrettyWriter.h json/JsonCompactWriter.h)