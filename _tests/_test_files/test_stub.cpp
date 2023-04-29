#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>

//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/sql/sql.h"
//------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------
//          COPY BASIC_TEST INTO THIS FILE.
//                    AND THEN,
//         DO NOT EDIT THIS FILE ANY FURTHER
//------------------------------------------------------------------------------
const vector<string> command_list = {
    "create table studentSample fields 	fname, 			lname, 		major, 				age",
    "insert into studentSample values 	Flo, 			Yao, 		CS, 				20",
    "insert into studentSample values 	\"Flo\", 			\"Jackson\", 	Math,	 			21",
    "insert into studentSample values 	Calvin, 		Woo, 		Physics,			22",
    "insert into studentSample values 	\"Anna Grace\", 	\"Del Rio\", 	CS,	 				22",
};

const vector<string> select_list =
    {
        "select * from studentSample where lname > Yang",

        "select * from studentSample where age > 50",

        "select * from studentSample where age  > 53",

        "select * from studentSample where age > 54",

        "select * from studentSample where lname >= Yang",

        "select * from studentSample where lname < Jackson",
        "select * from studentSample where lname <= Smith",
        "select * from studentSample where fname = \"Flo\" and lname = \"Yao\"",
        "select * from studentSample where fname = Flo or lname = Jackson",
        "select * from studentSample where fname = Flo or major = CS and age <= 23",
        "select * from studentSample where age <30 and major=CS or major = Physics and lname = Jackson",
        "select * from studentSample where lname = Yang or major = CS and age < 23 or lname = Jackson",

        // "select * from employeeSample01 where last >= Jack",

        // //.................
        // //:Less Than      :
        // //.................
        // //. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . .
        // select * from studentSample01 where lname < Jackson

        // //. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . .
        // select * from studentSample01 where age  < 20

        // //. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . .
        // select * from studentSample01 where age  < 19

        // //.................
        // //:Less Equal     :
        // //.................
        // select * from studentSample01 where lname <= Smith

        // //. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .
        // select * from employeeSample01 where last <= Peach

        // //******************************************************************************
        // //		LOGICAL OPERATORS
        // //******************************************************************************

        // //.................
        // //:AND            :
        // //.................
        // select * from studentSample01 where fname = "Flo" and lname = "Yao"

        // //.................
        // //:OR            :
        // //.................
        // select * from studentSample01 where fname = Flo or lname = Jackson

        // //.................
        // //:OR AND         :
        // //.................
        // select * from studentSample01 where fname = Flo or major = CS and age <= 23

        // //.................
        // //:AND OR AND     :
        // //.................
        // select * from studentSample01 where age <30 and major=CS or major = Physics and lname = Jackson

        // //.................
        // //:OR AND OR      :
        // //.................
        // select * from studentSample01 where lname = Yang or major = CS and age < 23 or lname = Jackson
};

bool test_stub(bool debug = false)
{
    remove("studentSample.bin");
    remove("studentSample_fields.bin");
    SQL sql;
    for (int i = 0; i < 5; ++i)
    {
        cout << sql.command(command_list[i]) << endl;
        cout << endl
             << endl;
    }
    for (int i = 0; i < select_list.size(); ++i)
    {
        cout << select_list[i] << endl;
        // cout << sql.command(select_list[i]) << endl
        //      << endl
        //      << endl;
        Table t = sql.command(select_list[i]);
        cout << t << endl
             << endl
             << endl;
        cout << "size:" << t.select_recnos().size() << endl;
        cout << "selected recnos:" << t.select_recnos() << endl;
    }
    remove("studentSample.bin");
    remove("studentSample_fields.bin");
}

// Lord help me!
const bool debug = false;

TEST(TEST_STUB, TestStub)
{
    bool success = test_stub(debug);
    EXPECT_EQ(success, true);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running instruction.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
