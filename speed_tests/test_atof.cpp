#include <cstdlib>
#include <vector>

int main(int argc, char** argv) {
    std::vector<double> result;
    
    for(int i=0;i<1024*1024;++i) {
        result.clear();
        result.reserve(39);
        result.push_back(atof("0"));
        result.push_back(atof("1.354"));
        result.push_back(atof("1054E-3"));
        result.push_back(atof("-1054E-3"));
        result.push_back(atof("-10.54E30"));
        result.push_back(atof("-10.54E45"));
        result.push_back(atof("-345554.54e-5"));
        result.push_back(atof("-34555.534954e-5"));
        result.push_back(atof("-34555.534954e-5"));
        result.push_back(atof("549067"));
        result.push_back(atof("567"));
        result.push_back(atof("446"));
        result.push_back(atof("7"));
        result.push_back(atof("73"));
        result.push_back(atof("256"));
        result.push_back(atof("5676"));
        result.push_back(atof("3"));
        result.push_back(atof("738"));
        result.push_back(atof("684"));
        result.push_back(atof("26"));
        result.push_back(atof("673.678e-56"));
        result.push_back(atof("53"));
        result.push_back(atof("67"));
        result.push_back(atof("684"));
        result.push_back(atof("-5437E24"));
        result.push_back(atof("8"));
        result.push_back(atof("84"));
        result.push_back(atof("3"));
        result.push_back(atof("56733.68"));
        result.push_back(atof("786"));
        result.push_back(atof("6478"));
        result.push_back(atof("34563.65683598734"));
        result.push_back(atof("5673"));
        result.push_back(atof("784e-3"));
        result.push_back(atof("6"));
        result.push_back(atof("5"));
        result.push_back(atof("8678"));
        result.push_back(atof("46784"));
        result.push_back(atof("-54.0888e-6"));
        result.push_back(atof("784"));
    }
}
