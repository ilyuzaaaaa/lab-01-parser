//
// Created by Илюза Янгирова on 17.09.2020.
//

#include "Parser.hpp"

#include <algorithm>
const size_t addProbel = 1;
JSONParser::JSONParser() = default;
JSONParser::JSONParser(const std::string &path) { parser(path); }
const std::vector<Student> &JSONParser::getStudents() const { return students; }
const FieldLengths &JSONParser::getL() const { return l; }

void JSONParser::printRow(std::ostream &out, const Student &student) const {
  out << std::left << "|" << std::setw(l.Field_1) << student.getName()
      << "|";
  if (std::any_cast<json>(student.getGroup()).is_number()) {
    out << std::setw(l.Field_2)
        << std::any_cast<json>(student.getGroup()).get<int>();
  } else if (std::any_cast<json>(student.getGroup()).is_string()) {
    out << std::setw(l.Field_2)
        << std::any_cast<json>(student.getGroup()).get<std::string>();
  } else {
    throw std::invalid_argument(
        "The type of the Group variable is undefined!!!");
  }
  out << "|" << std::setprecision(3) << std::setw(l.Field_3)
      << student.getAvg() << "|";
  if (std::any_cast<json>(student.getDebt()).is_null()) {
    out << std::setw(l.Field_4) << "null";
  } else if (std::any_cast<json>(student.getDebt()).is_array()) {
    std::string it = std::to_string(std::any_cast<json>(student.getDebt())
                                        .get<std::vector<std::string>>()
                                        .size()) +
                     " items";
    out << std::setw(l.Field_4) << it;
  } else if (std::any_cast<json>(student.getDebt()).is_string()) {
    out << std::setw(l.Field_4)
        << std::any_cast<json>(student.getDebt()).get<std::string>();
  } else {
    throw std::invalid_argument(
        "The type of the Debt variable is undefined!!!");
  }
  out << "|";
}
std::string JSONParser::getSeparator() const {
  std::string sep = "|";
  for (size_t i = 0; i < l.Field_1; ++i) {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < l.Field_2; ++i) {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < l.Field_3; ++i) {
    sep += "-";
  }
  sep += "|";
  for (size_t i = 0; i < l.Field_4; ++i) {
    sep += "-";
  }
  sep += "|";
  return sep;
}
void JSONParser::parser(const std::string &path) {
  if (path.empty()) {
    throw std::invalid_argument("The file path cannot be empty!!!");
  }
  std::ifstream json_file(path);
  if (!json_file.is_open()) {
    throw std::out_of_range("The file with the specified name: " + path +
                            " does not exist!!!");
  }
  json data;
  json_file >> data;
  if (!data.at("items").is_array()) {
    throw std::invalid_argument("Items is not array!!!");
  }
  if (data.at("items").size() != data.at("_meta").at("count").get<size_t>()) {
    throw std::out_of_range("Items length don't equal _meta.count!!!");
  }
  for (auto const &student : data.at("items")) {
    students.emplace_back(student);
  }
  setLengths();
}
void JSONParser::printData() {
  std::cout << *this;
}
bool JSONParser::emptyJSONobject() const { return students.empty(); }
void JSONParser::setJSONString(const std::string &JSON) {
  json data = json::parse(JSON);
  if (!data.at("items").is_array()) {
    throw std::invalid_argument("Items is not array!!!");
  }
  if (data.at("items").size() != data.at("_meta").at("count").get<size_t>()) {
    throw std::invalid_argument("Items length don't equal _meta.count!!!");
  }
  for (auto const &student : data.at("items")) {
    students.emplace_back(student);
  }
  setLengths();
}
void JSONParser::setLengths() {
  for (const auto &student : students) {
    if (student.getName().size() > l.Field_1) {
      l.Field_1 = student.getName().size() + addProbel;
    }
    if (std::any_cast<json>(student.getGroup()).is_number()) {
      if (std::to_string(std::any_cast<json>(student.getGroup()).get<int>())
              .size() > l.Field_2) {
        l.Field_2 =
            std::to_string(std::any_cast<json>(student.getGroup()).get<int>())
                .size() +
            addProbel;
      }
    } else {
      if (std::any_cast<json>(student.getGroup()).get<std::string>().size() >
          l.Field_2) {
        l.Field_2 =
            std::any_cast<json>(student.getGroup()).get<std::string>().size() +
            addProbel;
      }
    }
    if (std::any_cast<json>(student.getDebt()).is_string() &&
        std::any_cast<json>(student.getDebt()).get<std::string>().size() >
        l.Field_4) {
      l.Field_4 =
          std::any_cast<json>(student.getDebt()).get<std::string>().size() + addProbel;
    }
  }
}
std::ostream &operator<<(std::ostream &out, JSONParser &p) {
  out << std::left << "|" << std::setw(p.l.Field_1) << "name"
      << "|" << std::setw(p.l.Field_2) << "group"
      << "|" << std::setw(p.l.Field_3) << "avg"
      << "|" << std::setw(p.l.Field_4) << "debt"
      << "|" << '\n';
  std::string separator = p.getSeparator();
  out << separator << "\n";
  for (const auto &student : p.students) {
    p.printRow(out, student);
    out << '\n';
    out << separator << "\n";
  }
  return out;
}
JSONParser::~JSONParser() = default;
