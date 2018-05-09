//#include <cpprest/http_client.h>
#include <iostream>

#include <boost/type_index.hpp>
#include <restc-cpp/restc-cpp.h>

#include <restclient-cpp/restclient.h>

//const std::string c_request("https://here-hdmap-ext-weu-1.catalogs.datastore.api.here.com/v2/catalog/partition/lane-geometry-polyline/377783053?version=1811&app_id=NrbW64cyc6kEP6JSiB8c&app_code=CPRIfUarF5dPD-5e8jk75A");

const std::string request02("https://here-hdmap-ext-weu-1.catalogs.datastore.api.here.com/v2/catalog/partition/lane-geometry-polyline/377782698?version=1811&app_id=NrbW64cyc6kEP6JSiB8c&app_code=CPRIfUarF5dPD-5e8jk75A");

const std::string c_request("https://here-hdmap-ext-weu-1.catalogs.datastore.api.here.com/v2/catalog/partition/lane-geometry-polyline/377783053?version=1811&app_id=NrbW64cyc6kEP6JSiB8c&app_code=CPRIfUarF5dPD-5e8jk75A");

void loadLanes(restc_cpp::Context& ctx)
{

    auto reply = ctx.Get(c_request);
    auto data = reply->GetBodyAsString();

    std::cout << "Got data size: " << data.size() << std::endl;
}

int main()
{
    //auto rest_client = restc_cpp::RestClient::Create();

    //rest_client->Process(loadLanes);

    //rest_client->CloseWhenReady(true);

    auto responce = RestClient::get(c_request);
    std::cout << boost::typeindex::type_id<decltype(responce.body)>().pretty_name() << std::endl;

    std::cout << responce.code << std::endl;
    std::cout << responce.body << std::endl;

    for (const auto& str_pair : responce.headers)
    {
        std::cout << str_pair.first << " : " << str_pair.second << std::endl;
    }

    auto location_it = responce.headers.find("Location");
    if (location_it != responce.headers.end())
    {
        std::cout << "Data location: " << location_it->second << std::endl;

        auto data_responce = RestClient::get(location_it->second);

        std::cout << data_responce.code << std::endl;

        std::cout << "data size: " << data_responce.body.size() << std::endl;
        //if (argc >= 2)
        //{
            //std::ofstream fout();
        //}
        //std::cout << data_responce.body << std::endl;
        for (const auto& str_pair : data_responce.headers)
        {
            std::cout << str_pair.first << " : " << str_pair.second << std::endl;
        }
    }

    return 0;
}
