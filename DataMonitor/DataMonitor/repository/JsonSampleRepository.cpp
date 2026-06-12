#include "JsonSampleRepository.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace {

void skipWs(const std::string& s, size_t& pos) {
    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos])))
        ++pos;
}

std::string readString(const std::string& s, size_t& pos) {
    if (pos >= s.size() || s[pos] != '"')
        throw std::runtime_error("Expected '\"' at pos " + std::to_string(pos));
    ++pos;
    std::string result;
    while (pos < s.size() && s[pos] != '"') {
        if (s[pos] == '\\' && pos + 1 < s.size()) {
            ++pos;
            switch (s[pos]) {
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                case 'n':  result += '\n'; break;
                case 't':  result += '\t'; break;
                case 'r':  result += '\r'; break;
                default:   result += s[pos]; break;
            }
        } else {
            result += s[pos];
        }
        ++pos;
    }
    if (pos >= s.size())
        throw std::runtime_error("Unterminated string in JSON");
    ++pos;
    return result;
}

std::string readNumber(const std::string& s, size_t& pos) {
    size_t start = pos;
    if (pos < s.size() && s[pos] == '-') ++pos;
    while (pos < s.size() &&
           (std::isdigit(static_cast<unsigned char>(s[pos])) ||
            s[pos] == '.' || s[pos] == 'e' || s[pos] == 'E' ||
            s[pos] == '+' || s[pos] == '-')) {
        ++pos;
    }
    return s.substr(start, pos - start);
}

void skipValue(const std::string& s, size_t& pos);

void skipObject(const std::string& s, size_t& pos) {
    if (pos >= s.size() || s[pos] != '{') return;
    ++pos;
    int depth = 1;
    while (pos < s.size() && depth > 0) {
        if      (s[pos] == '{') ++depth;
        else if (s[pos] == '}') --depth;
        else if (s[pos] == '"') { readString(s, pos); continue; }
        ++pos;
    }
}

void skipArray(const std::string& s, size_t& pos) {
    if (pos >= s.size() || s[pos] != '[') return;
    ++pos;
    int depth = 1;
    while (pos < s.size() && depth > 0) {
        if      (s[pos] == '[') ++depth;
        else if (s[pos] == ']') --depth;
        else if (s[pos] == '"') { readString(s, pos); continue; }
        ++pos;
    }
}

void skipValue(const std::string& s, size_t& pos) {
    skipWs(s, pos);
    if (pos >= s.size()) return;
    if      (s[pos] == '"') readString(s, pos);
    else if (s[pos] == '{') skipObject(s, pos);
    else if (s[pos] == '[') skipArray(s, pos);
    else                    readNumber(s, pos);
}

Sample parseSample(const std::string& s, size_t& pos) {
    if (pos >= s.size() || s[pos] != '{')
        throw std::runtime_error("Expected '{' for sample object");
    ++pos;

    Sample sample;
    while (pos < s.size()) {
        skipWs(s, pos);
        if (pos >= s.size()) break;
        if (s[pos] == '}') { ++pos; break; }
        if (s[pos] == ',') { ++pos; continue; }

        std::string key = readString(s, pos);
        skipWs(s, pos);
        if (pos >= s.size() || s[pos] != ':')
            throw std::runtime_error("Expected ':' after key '" + key + "'");
        ++pos;
        skipWs(s, pos);

        if (key == "id") {
            sample.id = readString(s, pos);
        } else if (key == "name") {
            sample.name = readString(s, pos);
        } else if (key == "avgProductionTime") {
            sample.avgProductionTime = std::stod(readNumber(s, pos));
        } else if (key == "yield") {
            sample.yield = std::stod(readNumber(s, pos));
        } else if (key == "stock") {
            sample.stock = std::stoi(readNumber(s, pos));
        } else {
            skipValue(s, pos);
        }
    }
    return sample;
}

} // namespace

JsonSampleRepository::JsonSampleRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

std::vector<Sample> JsonSampleRepository::getAll() {
    std::ifstream file(filePath_);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filePath_);

    std::ostringstream ss;
    ss << file.rdbuf();
    const std::string content = ss.str();

    std::vector<Sample> samples;
    size_t pos = 0;

    const size_t samplesKey = content.find("\"samples\"");
    if (samplesKey == std::string::npos)
        return samples;

    pos = samplesKey + 9;
    skipWs(content, pos);
    if (pos >= content.size() || content[pos] != ':')
        throw std::runtime_error("Expected ':' after 'samples'");
    ++pos;
    skipWs(content, pos);

    if (pos >= content.size() || content[pos] != '[')
        throw std::runtime_error("Expected '[' for samples array");
    ++pos;

    while (pos < content.size()) {
        skipWs(content, pos);
        if (pos >= content.size()) break;
        if (content[pos] == ']') break;
        if (content[pos] == ',') { ++pos; continue; }
        if (content[pos] == '{')
            samples.push_back(parseSample(content, pos));
        else
            ++pos;
    }

    return samples;
}
