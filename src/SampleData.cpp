#include "SampleData.hpp"
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;

void generateSampleJSON(const std::string& filename) {
    json j;

    // 1. Modules
    j["modules"] = json::array({
      { { "code","ITP101" }, { "name","Introduction to Programming" }, { "desc","Basic C++ and problem solving" } },
      { { "code","DS102"  }, { "name","Data Structures"            }, { "desc","Lists, trees, graphs"       } },
      { { "code","DB103"  }, { "name","Database Systems"          }, { "desc","SQL and NoSQL fundamentals"} }
        });

    // 2. Rooms
    j["rooms"] = json::array({
      { { "id","MAE202" }, { "name","MAE 202 Lab"          } },
      { { "id","JCLT006"}, { "name","John Clare Theatre 006"} }
        });

    // 3. Session types
    j["session_types"] = json::array({
      { { "id","LEC" }, { "name","Lecture" } },
      { { "id","LAB" }, { "name","Lab"     } }
        });

    // 4. Groups
    j["groups"] = json::array({
      { { "id","GroupA" }, { "name","First‑year CS A" } },
      { { "id","GroupB" }, { "name","First‑year CS B" } }
        });

    // 5a. Admins
    j["admins"] = json::array({
      { { "id","admin1" }, { "name","John Scheduler" } }
        });

    // 5b. Students
    j["students"] = json::array({
      { { "id","s001" }, { "name","Alice" }, { "group","GroupA" } },
      { { "id","s002" }, { "name","Bob"   }, { "group","GroupB" } }
        });

    // 5c. Lecturers
    j["lecturers"] = json::array({
      {
        { "id","l001" },
        { "name","DrSmith" },
        { "modules", json::array({"ITP101"}) }
      },
      {
        { "id","l002" },
        { "name","DrJones" },
        { "modules", json::array({"DS102","DB103"}) }
      }
        });

    // 6. Timetables (weeks 1–26, with two conflicting sessions)
    j["timetables"] = json::array();
    for (int wk = 1; wk <= 26; ++wk) {
        json tj;
        tj["week"] = wk;
        tj["sessions"] = json::array();

        // Session A: GroupA lecture
        tj["sessions"].push_back({
          {"courseName","ITP101"},{"sessionType","LEC"},{"day","Monday"},
          {"startTime",600},{"endTime",660},
          {"roomId","MAE202"},{"lecturerId","l001"},{"groupId","GroupA"}
            });

        // Session B: conflicting same slot & room for GroupB
        tj["sessions"].push_back({
          {"courseName","ITP101"},{"sessionType","LEC"},{"day","Monday"},
          {"startTime",600},{"endTime",660},
          {"roomId","MAE202"},{"lecturerId","l001"},{"groupId","GroupB"}
            });

        // Session C: non‑conflicting DS102 lab
        tj["sessions"].push_back({
          {"courseName","DS102"},{"sessionType","LAB"},{"day","Wednesday"},
          {"startTime",840},{"endTime",900},
          {"roomId","MAE202"},{"lecturerId","l002"},{"groupId","GroupB"}
            });

        j["timetables"].push_back(std::move(tj));
    }

    std::ofstream ofs(filename);
    ofs << j.dump(2);
}
