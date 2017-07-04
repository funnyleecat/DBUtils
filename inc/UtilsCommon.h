#pragma once
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <ocilib.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include "ExportDef.hpp"
#include <set>
#include <boost/variant.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <CommonTemplate.h>
#include <boost/make_shared.hpp>

using namespace ocilib;
using namespace Utils;
typedef boost::shared_mutex RWMutex;
typedef boost::shared_lock<RWMutex> ReadLock;
typedef boost::unique_lock<RWMutex> WriteLock;
typedef boost::mutex::scoped_lock Scopedlock;

typedef boost::signals2::signal<void()> SigVoid;


#ifdef WIN64
#ifdef _DEBUG
#pragma comment(lib, "../../../../lib/cross/ocilib-4.3.0/lib/win64/StaticRelease/ociliba.lib")
//#pragma comment(lib, "../../../../lib/cross/zookeeper-3.4.9/lib/win64/Debug/zookeeper_d.lib")
#pragma comment(lib, "../../../../lib/cross/tinyxml/x64/StaticDebug/tinyxml.lib")
#else
#pragma comment(lib, "../../../../lib/cross/ocilib-4.3.0/lib/win64/StaticRelease/ociliba.lib")
//#pragma comment(lib, "../../../../lib/cross/zookeeper-3.4.9/lib/win64/Release/zookeeper.lib")
#pragma comment(lib, "../../../../lib/cross/tinyxml/x64/StaticRelease/tinyxml.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "../../../../lib/cross/ocilib-4.3.0/lib/win32/StaticRelease/ociliba.lib")
//#pragma comment(lib, "../../../../lib/cross/zookeeper-3.4.9/lib/win32/Debug/zookeeper_d.lib")
#pragma comment(lib, "../../../../lib/cross/tinyxml/x86/StaticDebug/tinyxml.lib")
#else
#pragma comment(lib, "../../../../lib/cross/ocilib-4.3.0/lib/win32/StaticRelease/ociliba.lib")
//#pragma comment(lib, "../../../../lib/cross/zookeeper-3.4.9/lib/win32/Release/zookeeper.lib")
#pragma comment(lib, "../../../../lib/cross/tinyxml/x86/StaticRelease/tinyxml.lib")
#endif
#endif

namespace DBUtils
{

	std::string GetEnvVariable(const std::string &strVariName);
	DBType Convert(std::string const &str);
	template<typename T>
	T &Ref(T *ptr)
	{
		if (ptr != NULL)
		{
			return *ptr;
		}
		else
		{
			throw;
		}
	}

	typedef std::list<std::string> StrList;
	typedef std::vector<std::string> StrVec;
	typedef std::vector<int> IntVec;
	typedef std::vector<double> DoubleVec;

	template<typename T>
	struct BindInType
	{
		public:
			BindInType()
			{

			}
			BindInType(T &value, bool bBindEmpty)
				:bindEmpty(bBindEmpty)
			{
				valueVec.push_back(value);
			}
		std::vector<T> valueVec;
		bool bindEmpty;
	};

	class CDBIterator;
	typedef boost::shared_ptr<CDBIterator> CDBIteratorSPtr;

	//泛型类型绑定入参类型绑定容器
	typedef boost::variant<int, double, std::string> VariantType;
	
	typedef boost::variant<BindInType<int>, BindInType<double>, BindInType<std::string> > VariantBindInType;
	typedef std::map<std::string, VariantBindInType> StrVariantBindInTypeMap;

	typedef std::pair<const std::string, CDBIteratorSPtr> CDBIteratorSPtrPair;
	typedef std::map<const std::string, CDBIteratorSPtr> CDBIteratorSPtrMap;

	typedef std::set<std::string> StrSet;

	template<typename T>
	struct BindInOutType
	{
		T value;
		BindDirecation bindDirect;
	};

	//泛型类型绑定出入参类型绑定容器
	typedef boost::variant<BindInOutType<int>, BindInOutType<double>, BindInOutType<std::string> > VariantBindInOutType;
	typedef std::map<std::string, VariantBindInOutType> StrVariantBindInOutTypeMap;

	//class 
	enum DBRWType
	{
		RO,
		WO,
		WR_Unknown
	};

	DBRWType ConvertRWType(std::string const& str);
	
}
