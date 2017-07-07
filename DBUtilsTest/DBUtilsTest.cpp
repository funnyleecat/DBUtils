// DBUtilsTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ExportDef.hpp"
#include "DBIterator.h"
#include "Statement.h"
#include "DBMgr.hpp"
#include <iostream>
#include <sstream>
#include <DBExpection.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "stdio.h"
#include <memory>
#include <boost/variant.hpp>
#include <boost/signals2.hpp>

using namespace DBUtils;
//pass
void Test_Cmd_Insert()
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", R_Mode, 3);
	st->BindCmd("insert into StudentdInfo values(1,'lifeng',22)");
	try
	{
		ref.Execute(st, 1);
		ref.Commit(st);
		ref.Release(st);
	}
	catch (CDBExpection & exp)
	{
		ref.Release(st);
		cout << "error:" << exp.GetErrorName() << endl;
	}

}

//pass
void Test_Cmd_Insert_byId(long id)
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", W_Mode, 3);
	std::ostringstream os;
	os << "insert into StudentdInfo values(" << id << ",'lifeng',22)";
	st->BindCmd(os.str().c_str());

	try
	{
		ref.Execute(st, 2);
		ref.Commit(st);
		ref.Release(st);
	}
	catch (CDBExpection & exp)
	{
		ref.Release(st);
		cout << "error:" << exp.GetErrorName() << endl;
	}

}


//pass
void Test_Cmd_delete()
{
	try {
		CDBMgr &ref = CDBMgr::Instance();
		CDBStatement *st = ref.AllocStatement("lifengDB", W_Mode, 3);
		st->BindCmd("delete from StudentdInfo");
		ref.Execute(st);
		ref.Commit(st);
		ref.Release(st);
	}
	catch (CDBExpection & exp)
	{
		cout << exp.GetErrorName() << endl;
	}

}


//pass
void Test_Buld_Insert()
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", W_Mode, 3);
	st->BindCmd("insert into StudentdInfo(IDNUM, SNAME, SAGE) values(:IDNUM, :SNAME, :SAGE)");


	for (int i = 1; i < 5; ++i)
	{
		st->BindInt("IDNUM", i, D_IN);
		st->BindString("SNAME", "lifeng", D_IN);
		int age = 21;
		st->BindInt("SAGE", age, D_IN);
	}

	try{
		ref.Execute(st, 1);
		ref.Commit(st);
		ref.Release(st);
	}
	catch (CDBExpection & exp)
	{
		cout << "error:" << exp.GetErrorName() << endl;
		ref.Release(st);
	}
}


//pass
void Test_View_data()
{
	try
	{
		CDBMgr &ref = CDBMgr::Instance();
		CDBStatement *st = ref.AllocStatement("lifengDB", R_Mode, 3);
		st->BindCmd("select *from StudentdInfo");
		bool rst = ref.Execute(st, 1);
		CDBIterator *iter = st->GetCursorRst("");
		int i = 0;
		if (iter != NULL)
		{
			unsigned int columnCount = iter->GetColumnCount();
			while (iter->Next())
			{
				for (unsigned int index = 1; index <= columnCount; ++index)
				{
					const char *p = iter->At(index);
					std::string rst(p);
					cout << rst << " ";
				}

				cout << endl;
			}

		}
		ref.Release(st);
	}
	catch (std::exception &exp)
	{
		cout << exp.what() << endl;
	}
}


//pass
void Test_View_ByBind()
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", R_Mode, 3);
	st->BindCmd("select * from StudentdInfo where IDNUM = :IDNum");
	st->BindInt("IDNum", 1, D_IN);
	bool rst = ref.Execute(st, 1);
	CDBIterator *iter = st->GetCursorRst("");
	if (iter != NULL)
	{
		unsigned int columnCount = iter->GetColumnCount();
		while (iter->Next())
		{
			for (unsigned int index = 1; index <= columnCount; ++index)
			{
				const char *p = iter->At(index);
				std::string rst(p);
				cout << rst << " ";
			}
			cout << endl;
		}
	}
	ref.Release(st);
}

//pass
void Test_View_procedure()
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", R_Mode, 3);
	st->BindCmd("begin getAge(:id, :age); end;");
	st->BindInt("id", 1, D_IN);
	st->BindInt("age", 50, D_OUT);

	bool rst = ref.Execute(st, 1);
	CDBIterator *iter = st->GetCursorRst("");
	const char *rstNum = NULL;
	rstNum = st->GetBindOutResult("age", DT_INT);
	cout << std::string(rstNum) << endl;
}


//pass
void Test_Cursor_procedure()
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", R_Mode, 3);
	st->BindCmd("begin p_temp_procedure(:cur_arg); end;");
	st->BindOutCursor("cur_arg");

	bool rst = ref.Execute(st, 3);

	CDBIterator *output = st->GetCursorRst("cur_arg");
	if (output != NULL)
	{
		unsigned int columnCount = output->GetColumnCount();
		while (output->Next())
		{
			for (unsigned int index = 1; index <= columnCount; ++index)
			{
				const char *p = output->At(index);
				std::string rst(p);
				cout << rst << " ";
			}
			cout << endl;
		}
	}

}


//pass
void TestConnectingTest()
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", R_Mode, 3);
	CDBStatement *st1 = ref.AllocStatement("lifengDB", R_Mode, 3);

}

//pass
void TestExecuteEmpty()
{
	try
	{
		CDBMgr &ref = CDBMgr::Instance();
		CDBStatement *st = NULL;
		bool rst = ref.Execute(st, R_Mode);
	}
	catch (CDBExpection &exp)
	{
		printf("%s\n", exp.GetErrorName());
	}
	catch (...)
	{
		cout << "error" << endl;
	}
}

void TestFinitAllocate()
{

	try
	{
		CDBMgr &ref = CDBMgr::Instance();
		ref.Finit();

		CDBStatement *st = ref.AllocStatement("LifengDB", W_Mode, 3);
		st->BindCmd("insert into DBTest values(1,'lifeng',22)");

		bool rst = ref.Execute(st, 1);
		ref.Commit(st);
		ref.Release(st);
	}
	catch (CDBExpection &exp)
	{
		printf("%s\n", exp.GetErrorName());
	}
	catch (...)
	{
		cout << "error" << endl;
	}
}



void TestBindNull()
{
	CDBMgr &ref = CDBMgr::Instance();
	CDBStatement *st = ref.AllocStatement("lifengDB", R_Mode, 3);
	st->BindCmd("select * from StudentdInfo where IDNUM = :IDNum or :IDNum is null");
	st->BindInt("IDNum", 1, D_IN, true);
	bool rst = ref.Execute(st, 5);
	CDBIterator *iter = st->GetCursorRst("");
	if (iter != NULL)
	{
		unsigned int columnCount = iter->GetColumnCount();
		while (iter->Next())
		{
			for (unsigned int index = 1; index <= columnCount; ++index)
			{
				const char *p = iter->At(index);
				std::string rst(p);
				cout << rst << " ";
			}
			cout << endl;
		}
	}
	ref.Release(st);
}


void InitDBEnv()
{

	CDBMgr &ref = CDBMgr::Instance();
	std::string file = "DBSvrCfg.xml";
	try
	{
		bool result = ref.Init(file);
	}
	catch (CDBExpection &exp)
	{
		printf("%s\n", exp.GetErrorName());


	}
}

void FinitDBEnv()
{
	CDBMgr &ref = CDBMgr::Instance();
	try
	{
		ref.Finit();
	}
	catch (CDBExpection &exp)
	{
		printf("%s\n", exp.GetErrorName());
	}
}

int main()
{
	InitDBEnv();
	Test_Cmd_delete();
	//TestFinitAllocate();
	Test_Buld_Insert();
	//TestFinitAllocate();
	//TestBindNull();
	//Test_Cmd_Insert();
	//Test_Cmd_Insert();
	Test_View_data();
	//Test_Cmd_delete();
	//TestMutiCmd();
	//TestConnectingTest();
	//Test_Cmd_Insert();
	//Test_Cmd_delete();
	//Test_View_data();
	//Test_Cursor_procedure();
	//Test_View_data();
	//Test_View_procedure();
	//Test_View_ByBind();
	//TestConnectingTest();
	//TestExecuteEmpty();
	//TestMutiThread();
	//TestTimerThread();
	//ref.Finit();
	FinitDBEnv();
	return 0;
}

