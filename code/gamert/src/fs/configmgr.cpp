#include "configmgr.hpp"
#include "tinyxml2.h"
#include "resmgr-static.hpp"

using namespace std;

const char* HardCoded_DefaultMasterConfigPath = "configs/master.xml";
const std::regex ConfigMgr::_srgx_prs_int32_attr0("\\{.*\\}");
const std::regex ConfigMgr::_srgx_prs_int32_attr1("\\$\\w*");

ConfigMgr::ConfigMgr()
{}

void ConfigMgr::load_config()
{
	_load_cfg_master();

#define LOAD_SUBCONFIG(name, load_func)		\
	{	\
		const auto& configpath = ConfigMaster::subconfigs.find(name);		\
		GRT_CHECK(configpath != ConfigMaster::subconfigs.end(), name " config not found!")	\
		load_func(configpath->second);		\
	}
	
	LOAD_SUBCONFIG("Networks", _load_cfg_networks);
	LOAD_SUBCONFIG("Dynopt", _load_cfg_dynopt);

#undef	LOAD_SUBCONFIG

}

void ConfigMgr::_load_cfg_master()
{
	try
	{
		tinyxml2::XMLDocument xdoc;
		xdoc.LoadFile(
			ResMgrStatic::get_instance()
			.fullpath(HardCoded_DefaultMasterConfigPath)
			.c_str());

		const auto& xmastercfg = xdoc.RootElement();

		if (GRT_IS_STRING_EQUAL(
			xmastercfg->Name(),
			"MasterConfig"))
		{
			_load_cfg_master_min_gameframe_interval(xmastercfg->LastChildElement("MinGameFrameInterval"));
			_load_cfg_master_subcfgs(xmastercfg->LastChildElement("SubConfigs"));
		}
		else
		{
			throw std::runtime_error(
				"unknown file format.");
		}
	}
	catch (std::exception ex)
	{
		// TODO: handle exceptions
		throw ex;
	}
}

void ConfigMgr::_load_cfg_master_subcfgs(const void* xnode)
{
	const tinyxml2::XMLElement& xsubcfgs = *((const tinyxml2::XMLElement*)xnode);
	const tinyxml2::XMLElement* xscfg = xsubcfgs.FirstChildElement();

	ConfigMaster::subconfigs.clear();

	while (nullptr != xscfg)
	{
		if (GRT_IS_STRING_EQUAL(
			xscfg->Name(),
			"Config"))
		{
			const char* cname = xscfg->Attribute("Name");
			const char* cfile = xscfg->Attribute("File");
			
			ConfigMaster::subconfigs[cname] = cfile;
		}
		xscfg = xscfg->NextSiblingElement();
	}
}

void ConfigMgr::_load_cfg_master_min_gameframe_interval(const void* xnode)
{
	const tinyxml2::XMLElement& xmingameframeitvl = *((const tinyxml2::XMLElement*)xnode);
	ConfigMaster::min_gameframe_interval = xmingameframeitvl.FloatAttribute("Value");
}

void ConfigMgr::_load_cfg_networks(const std::string& configpath)
{
	try
	{
		tinyxml2::XMLDocument xdoc;
		xdoc.LoadFile(
			ResMgrStatic::get_instance()
			.fullpath(configpath)
			.c_str());

		const auto& xnetworkscfg = xdoc.RootElement();

		if (GRT_IS_STRING_EQUAL(
			xnetworkscfg->Name(),
			"NetworksConfig"))
		{
			_load_cfg_networks_constants(xnetworkscfg);
			_load_cfg_networks_connections(xnetworkscfg);
		}
		else
		{
			throw std::runtime_error(
				"unknown file format.");
		}
	}
	catch (std::exception ex)
	{
		// TODO: handle exceptions
		throw ex;
	}
}

void ConfigMgr::_load_cfg_networks_constants(const void* xnetworkscfg)
{
	const tinyxml2::XMLElement& xntcfg = *((const tinyxml2::XMLElement*)xnetworkscfg);
	const  tinyxml2::XMLElement* xiter = xntcfg.FirstChildElement();
	while (nullptr != xiter)
	{
		if (GRT_IS_STRING_EQUAL(
			xiter->Name(),
			"Const"))
		{
			if (GRT_IS_STRING_EQUAL(
				xiter->Attribute("Type"),
				"int32"))
			{
				ConfigNetworks::constants[xiter->Attribute("Name")] =
					xiter->IntAttribute("Value");
			}
		}
		xiter = xiter->NextSiblingElement();
	}
}


void ConfigMgr::_load_cfg_networks_connections(const void* xnetworkscfg)
{
	const tinyxml2::XMLElement& xntcfg = *((const tinyxml2::XMLElement*)xnetworkscfg);
	const  tinyxml2::XMLElement* xiter = xntcfg.FirstChildElement();
	ConfigNetworks::connections.clear();
	while (nullptr != xiter)
	{
		if (GRT_IS_STRING_EQUAL(
			xiter->Name(),
			"Connection"))
		{
			ConfigNetworks::connection_t	conn;
			conn.remote_ipaddr = xiter->Attribute("RemoteAddr");
			conn.port = (uint16_t)xiter->IntAttribute("Port");
			conn.enable_heartbeats = xiter->BoolAttribute("EnableHeartbeats");
			conn.server_type = _parse_int32_attr(xiter->Attribute("ServerType"),
				ConfigNetworks::constants);

			ConfigNetworks::connections.push_back(conn);
		}

		xiter = xiter->NextSiblingElement();
	}
}

int32_t ConfigMgr::_parse_int32_attr(
	const char* attr,
	const std::unordered_map<std::string, std::any>& dict)
{
	int32_t res = 0;
	std::cmatch	cm_script;
	if (std::regex_match(
		attr,
		cm_script,
		_srgx_prs_int32_attr0,
		std::regex_constants::match_not_null))
	{ // attr is a script
		//TODO: use lua as the script language, now we use regex matching for the prototype
		std::string script(
			cm_script[0].str().c_str() + 1,
			cm_script[0].length() - 2);

		std::smatch sm_valname;
		if (std::regex_match(
			script,
			sm_valname,
			_srgx_prs_int32_attr1
		))
		{
			std::string name(
				sm_valname[0].str().c_str() + 1,
				sm_valname[0].length() - 1);
			const auto& hit = dict.find(name);
			if (hit != dict.cend() &&
				hit->second.type() == typeid(int32_t))
			{
				res = std::any_cast<int32_t>(hit->second);
			}
		}
	}
	else
	{ // attr is plain text
		std::stringstream	ss;
		ss << attr;
		ss >> res;
	}

	return res;
}

void ConfigMgr::_load_cfg_dynopt(const std::string& configpath)
{
	try
	{
		tinyxml2::XMLDocument xdoc;
		xdoc.LoadFile(
			ResMgrStatic::get_instance()
			.fullpath(configpath)
			.c_str());

		const auto& xdynopt = xdoc.RootElement();

		if (GRT_IS_STRING_EQUAL(
			xdynopt->Name(),
			"DynoptConfig"))
		{
			const auto& xluart = xdynopt->LastChildElement("LuaRT");
			if (xluart)
			{
				_load_cfg_dynopt_luart(xluart);
			}

		}
		else
		{
			throw std::runtime_error(
				"unknown file format.");
		}
	}
	catch (std::exception ex)
	{
		// TODO: handle exceptions
		throw ex;
	}
}

void ConfigMgr::_load_cfg_dynopt_luart(const void* xluart)
{
	const tinyxml2::XMLElement& xlua = *((const tinyxml2::XMLElement*)xluart);

	const auto& xreqlist = xlua.LastChildElement("RequireList");
	if (xreqlist)
	{
		_load_cfg_dynopt_luart_requirelist(xreqlist);
	}
}

void ConfigMgr::_load_cfg_dynopt_luart_requirelist(const void* xrequirelist)
{
	const tinyxml2::XMLElement& xlist = *((const tinyxml2::XMLElement*) xrequirelist);

	auto xitem = xlist.FirstChildElement();
	while (xitem)
	{
		if (GRT_IS_STRING_EQUAL(
			xitem->Name(),
			"Require"))
		{
			const char* rname = xitem->GetText();
			if (rname)
			{
				ConfigDynopt::luart::require_list.push_back(rname);
			}
		}

		xitem = xitem->NextSiblingElement();
	}

}




