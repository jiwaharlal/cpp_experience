#include <iostream>
#include <string>

int main()
{
    const std::string specifier = "<<ProgramFileName>>";
    std::string coded_path = "/c/ProgramData/Materialise/LicenseFiles/logs/<<ProgramFileName>>/logfile.log";
    std::cout << coded_path << std::endl;

    const auto pos = coded_path.find(specifier);
    if (pos != std::string::npos)
    {
        const std::string app_name = "mimics.exe";
        coded_path.replace(pos, specifier.length(), app_name);
        std::cout << coded_path << std::endl;
    }

    return 0;
}
