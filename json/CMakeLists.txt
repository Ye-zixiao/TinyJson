cmake_minimum_required(VERSION 3.16)

add_library(TinyJson SHARED
        Document.h
        FileReadStream.h
        FileWriteStream.h
        JsonBaseWriter.h
        JsonCompactWriter.h
        JsonPrettyWriter.h
        JsonReader.h
        JsonReader.cpp
        JsonValue.h
        JsonValue.cpp
        noncopyable.h
        ParseException.h
        StringReadStream.h
        StringWriteStream.h)