//Copyright [year] <Copyright Owner>

#ifndef INCLUDE_JSONPARSER_HPP_
#define INCLUDE_JSONPARSER_HPP_

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Student.hpp"

struct FieldLengths {
  size_t Field_1;
  size_t Field_2;
  size_t Field_3;
  size_t Field_4;
};
class JSONParser {
 public:
  JSONParser();
  explicit JSONParser(const std::string &path);
  ~JSONParser();
  const std::vector<Student> &getStudents() const;
  const FieldLengths &getLengths() const;
  void printRow(std::ostream &out, const Student &student) const;
  std::string getSeparator() const;
  void parser(const std::string &path);
  bool emptyJSONobject() const;

  friend std::ostream &operator<<(std::ostream &out, JSONParser &p);

 private:
  std::vector<Student> students;
  FieldLengths l{15, 8, 6, 15};
  void setLengths();
};

#endif  // INCLUDE_JSONPARSER_HPP_
