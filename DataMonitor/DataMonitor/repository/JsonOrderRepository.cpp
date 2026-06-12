#include "JsonOrderRepository.h"
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

void skipValue(const std::string& s, size_t& pos) {
    skipWs(s, pos);
    if (pos >= s.size()) return;
    if (s[pos] == '"') {
        readString(s, pos);
    } else if (s[pos] == '{' || s[pos] == '[') {
        char open = s[pos], close = (open == '{') ? '}' : ']';
        ++pos;
        int depth = 1;
        while (pos < s.size() && depth > 0) {
            if      (s[pos] == open)  ++depth;
            else if (s[pos] == close) --depth;
            else if (s[pos] == '"')   { readString(s, pos); continue; }
            ++pos;
        }
    } else {
        readNumber(s, pos);
    }
}

// DataPersistence JSON schema: plain array of objects
// { "id", "sampleId", "customerName", "quantity", "status",
//   "actualProductionQuantity", "productionStartTime", "totalProductionTime" }
Order parseOrder(const std::string& s, size_t& pos) {
    if (pos >= s.size() || s[pos] != '{')
        throw std::runtime_error("Expected '{' for order object");
    ++pos;

    Order order;
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
            order.id = readString(s, pos);
        } else if (key == "sampleId") {
            order.sampleId = readString(s, pos);
        } else if (key == "customerName") {
            order.customerName = readString(s, pos);
        } else if (key == "quantity") {
            order.quantity = std::stoi(readNumber(s, pos));
        } else if (key == "status") {
            order.status = orderStatusFromString(readString(s, pos));
        } else if (key == "actualProductionQuantity") {
            order.actualProductionQuantity = std::stoi(readNumber(s, pos));
        } else if (key == "productionStartTime") {
            order.productionStartTime = readString(s, pos);
        } else if (key == "totalProductionTime") {
            order.totalProductionTime = std::stod(readNumber(s, pos));
        } else {
            skipValue(s, pos);
        }
    }
    return order;
}

std::vector<Order> parseArray(const std::string& content) {
    std::vector<Order> orders;
    size_t pos = 0;
    skipWs(content, pos);
    if (pos >= content.size() || content[pos] != '[')
        return orders;
    ++pos;

    while (pos < content.size()) {
        skipWs(content, pos);
        if (pos >= content.size()) break;
        if (content[pos] == ']') break;
        if (content[pos] == ',') { ++pos; continue; }
        if (content[pos] == '{')
            orders.push_back(parseOrder(content, pos));
        else
            ++pos;
    }
    return orders;
}

} // namespace

JsonOrderRepository::JsonOrderRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

std::vector<Order> JsonOrderRepository::getAll() {
    std::ifstream file(filePath_);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filePath_);

    std::ostringstream ss;
    ss << file.rdbuf();
    return parseArray(ss.str());
}

std::optional<Order> JsonOrderRepository::findById(const std::string& id) {
    const auto all = getAll();
    for (const auto& o : all) {
        if (o.id == id) return o;
    }
    return std::nullopt;
}
