//
// Created by Илюза Янгирова on 17.09.2020.
//

#ifndef INCLUDE_PARSER_HPP_
#define INCLUDE_PARSER_HPP_

#include "Student.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct FieldLengths {
    size_t Field_1,  Field_2;
    size_t Field_3;
    size_t Field_4;
};
class JSONParser {
public:
 JSONParser();
    explicit JSONParser(const std::string &path);
    ~JSONParser();
    const std::vector<Student> &getStudents() const;
     const FieldLengths &getL() const;
    void printRow(std::ostream& out, const Student &student) const;
     std::string getSeparator() const;
    void parser(const std::string &path);
    void printData();
     bool emptyJSONobject() const;
    void setJSONString(const std::string &JSON);
    friend std::ostream& operator <<(std::ostream& out, JSONParser & p);
private:
    std::vector<Student> students;
    FieldLengths l{15, 8, 6, 15};
    void setLengths();
};

#endif  // INCLUDE_PARSER_HPP_
