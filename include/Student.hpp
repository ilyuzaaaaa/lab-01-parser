//Copyright [year] <Copyright Owner>

#ifndef INCLUDE_STUDENT_HPP_
#define INCLUDE_STUDENT_HPP_

#include <any>
#include <exception>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
class Student {
public:
    explicit Student(const json &obj);
    Student();
     const std::string &getName() const;
    const std::any &getGroup() const;
     double getAvg() const;
    const std::any &getDebt() const;
    void from_json(const json &j);

private:
    std::string Name;
    std::any Group;
    double Avg = 0;
    std::any Debt;
};

#endif  // INCLUDE_STUDENT_HPP_
