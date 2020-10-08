// Copyright 2020 Evgenij Grigorev evgengrmit@icloud.com

#include <gtest/gtest.h>

#include <sstream>

#include "JSONParser.hpp"
// Tests for JSONParser
std::string getFullPath(const std::string& name) {
  std::string s(__FILE__);
  for (size_t i = 0; i < 8; ++i) {
    s.pop_back();
  }
  return s + name;
}
TEST(Parser, EmptyParser) {
  JSONParser a;
  ASSERT_TRUE(a.emptyJSONobject());
  JSONParser b;
  b.setJSONString(R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
  ASSERT_FALSE(b.emptyJSONobject());
}
TEST(Parser, Throw) {
  EXPECT_THROW(JSONParser(""), std::invalid_argument);
  EXPECT_THROW(JSONParser("Wrong.json"), std::out_of_range);
  EXPECT_THROW(JSONParser().setJSONString(R"({
  "items":
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    }
}
)"),
               std::invalid_argument);
  EXPECT_THROW(JSONParser().setJSONString(R"({
  "items": [
    {
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 4
  }
})"),
               std::invalid_argument);
}
TEST(Parser, LengthOfFields) {
  ASSERT_EQ(JSONParser().getLengths().Field_1, 15);
  ASSERT_EQ(JSONParser().getLengths().Field_2, 8);
  ASSERT_EQ(JSONParser().getLengths().Field_3, 6);
  ASSERT_EQ(JSONParser().getLengths().Field_4, 15);
  JSONParser b;
  b.setJSONString(R"({
  "items": [
    {
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    },
    {
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++ Java Python C#"
    },
    {
      "name": "Pertov Nikita",
      "group": "IU8-31-2019-2025",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    }
  ],
  "_meta": {
    "count": 3
  }
})");
  ASSERT_EQ(b.getLengths().Field_1, 22);  // 21+1
  ASSERT_EQ(b.getLengths().Field_2, 17);  // 16+1
  ASSERT_EQ(b.getLengths().Field_3, 6);
  ASSERT_EQ(b.getLengths().Field_4, 19);  // 18+1
}

TEST(Parser, Separator) {
  std::string s = "|---------------|--------|------|---------------|";
  ASSERT_EQ(JSONParser().getSeparator(), s);
}
TEST(Parser, PrintRow) {
  JSONParser p;
  Student s(json::parse(R"({
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })"));
  std::stringstream ss;
  p.printRow(ss, s);
  ASSERT_EQ(ss.str(), "|Ivanov Petr    |1       |4.25  |null           |");
}
TEST(Parser, ConstructorThrow) {
  ASSERT_THROW(JSONParser(getFullPath("ArraySizeWrong.json")),
               std::out_of_range);
  ASSERT_THROW(JSONParser(getFullPath("NoBracesForArray.json")),
               std::invalid_argument);
}

// Tests for Student
TEST(Student, CorrectTypesOfFieldsNull) {
  Student s{json::parse(R"({
      "name": "Ivanov Petr",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })")};
  ASSERT_EQ(s.getName(), "Ivanov Petr");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<std::string>(), "1");
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.25);
  ASSERT_TRUE(std::any_cast<json>(s.getDebt()).is_null());
}
TEST(Student, CorrectTypesOfFieldsString) {
  Student s{json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })")};
  ASSERT_EQ(s.getName(), "Sidorov Ivan");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<int>(), 31);
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.00);
  ASSERT_TRUE(std::any_cast<json>(s.getDebt()).is_string());
}
TEST(Student, CorrectTypesOfFieldsArray) {
  Student s{json::parse(R"({
      "name": "Pertov Nikita",
      "group": "IU8-31",
      "avg": 3.33,
      "debt": [
        "C++",
        "Linux",
        "Network"
      ]
    })")};
  ASSERT_EQ(s.getName(), "Pertov Nikita");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<std::string>(), "IU8-31");
  ASSERT_DOUBLE_EQ(s.getAvg(), 3.33);
  ASSERT_TRUE(std::any_cast<json>(s.getDebt()).is_array());
}
TEST(Student, ThrowStudent) {
  ASSERT_THROW(Student{json::parse(R"({})")}, std::invalid_argument);
}
TEST(Student, From_json_methods) {
  Student s, m;
  s.from_json(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": 4,
      "debt": "C++"
    })"));
  ASSERT_EQ(s.getName(), "Sidorov Ivan");
  ASSERT_EQ(std::any_cast<json>(s.getGroup()).get<int>(), 31);
  ASSERT_DOUBLE_EQ(s.getAvg(), 4.00);
  ASSERT_EQ(std::any_cast<json>(s.getDebt()).get<std::string>(), "C++");
  m.from_json(json::parse(R"({
      "name": "Ivanov Petr Petrovich",
      "group": "1",
      "avg": "4.25",
      "debt": null
    })"));
  ASSERT_EQ(m.getName(), "Ivanov Petr Petrovich");
  ASSERT_EQ(std::any_cast<json>(m.getGroup()).get<std::string>(), "1");
  ASSERT_DOUBLE_EQ(m.getAvg(), 4.25);
  ASSERT_TRUE(std::any_cast<json>(m.getDebt()).is_null());
}

TEST(Student, ThrowFromJsonMethod) {
  Student s;
  ASSERT_THROW(s.from_json(json::parse(R"({})")), std::invalid_argument);
  json j = json::parse(R"({
      "name": "Sidorov Ivan",
      "group": "31",
      "avg": [],
      "debt": "C++"
    })");
  ASSERT_THROW(s.from_json(j), std::invalid_argument);
}

TEST(Student, ThrowConstructor) {
  ASSERT_THROW(Student(json::parse(R"({})")), std::invalid_argument);
  ASSERT_THROW(Student(json::parse(R"({
      "name": "Sidorov Ivan",
      "group": 31,
      "avg": [],
      "debt": "C++"
    })")),
               std::invalid_argument);
}
