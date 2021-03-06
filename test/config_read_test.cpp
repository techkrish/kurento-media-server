/*
 * (C) Copyright 2015 Kurento (http://kurento.org/)
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 */

// #define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ConfigRead
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "loadConfig.hpp"

static void
print_ptree_info (boost::property_tree::ptree ptree, std::string name = "")
{
  std::stringstream ss;

  boost::property_tree::write_info (ss, ptree);

  std::cout << "---------------" << name << "---------------" << std::endl <<
            ss.str() << std::endl << "---------------" << std::endl ;
}

BOOST_AUTO_TEST_CASE ( merge_trees_with_array )
{
  std::string json_orig =
    "{\"noarrayorig\": 2, \"obj\": {\"array\":[5,6,7], \"array2\": [{\"p1\":1, "
    "\"p2\":2}]}}";
  std::stringstream json_orig_stream;
  json_orig_stream << json_orig;

  std::string json =
    "{"
    "\"noarray\": 1,\"obj\": { \"array\":[4,3,2], \"array2\": [{\"p1\":1, "
    "\"p2\":2}, {\"p3\":3, \"p4\":4}, {\"array3\": [10,11,12]}], \"array3\": "
    "[10,11,12]}}";
  std::stringstream json_stream;
  json_stream << json;

  std::string xml = "<obj>\n\n <group_key>value group</group_key>\n</obj>";
  std::stringstream xml_stream;
  xml_stream << xml;

  boost::property_tree::ptree ptree;
  boost::property_tree::ptree xmlptree;
  boost::property_tree::ptree mergedTree;

  boost::property_tree::read_json (json_stream, ptree);
  print_ptree_info (ptree, "json");
  boost::property_tree::read_json (json_orig_stream, mergedTree);
  print_ptree_info (mergedTree, "orig");
  boost::property_tree::read_xml (xml_stream, xmlptree);
  print_ptree_info (xmlptree, "xml");

  kurento::mergePropertyTrees (mergedTree, ptree);

  print_ptree_info (mergedTree, "merge_with_json");

  kurento::mergePropertyTrees (mergedTree, xmlptree);
  print_ptree_info (mergedTree, "merge_with_xml");


  {
    std::string expected =
      "noarrayorig 2\nnoarray 1\nobj\n{\n    array\n    {\n        \"\" 4\n    "
      "    \"\" 3\n        \"\" 2\n    }\n    array2\n    {\n        \"\"\n    "
      "    {\n            p1 1\n            p2 2\n        }\n        \"\"\n    "
      "    {\n            p3 3\n            p4 4\n        }\n        \"\"\n    "
      "    {\n            array3\n            {\n                \"\" 10\n     "
      "           \"\" 11\n                \"\" 12\n            }\n        }\n "
      "   }\n    array3\n    {\n        \"\" 10\n        \"\" 11\n        "
      "\"\" 12\n    }\n    group_key \"value group\"\n}\n";
    std::stringstream ss;
    boost::property_tree::write_info (ss, mergedTree);


    BOOST_CHECK_EQUAL (expected, ss.str() );
  }


}
