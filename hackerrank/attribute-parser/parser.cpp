#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

struct Tag
{
    std::string name;
    std::map<std::string, std::string> attrs;
    std::vector<Tag> nested;
};

std::pair<const char*, const char*> find_word_limits(const char* str_begin, const char* str_end)
{
    const char delim[] = " =<>/";
    const char* delim_end = delim + sizeof(delim);

    auto word_begin = std::find_if(
            str_begin,
            str_end,
            [&](const char& c){ return std::find(delim, delim_end, c) == delim_end; });

    auto word_end = std::find_if(
            word_begin,
            str_end,
            [&](const char& c){ return std::find(delim, delim_end, c) != delim_end; });

    return std::make_pair(word_begin, word_end);
}

bool isTag(const char* tag_line)
{
    const auto str_end = std::find(tag_line, tag_line + 201, 0);
    const auto tag_begin = std::find(tag_line, str_end, '<');
    const auto tag_end = std::find(tag_begin, str_end, '>');

    return tag_begin != str_end && tag_end != str_end;
}

bool isOpenTag(const char* tag_line)
{
    const auto str_end = std::find(tag_line, tag_line + 201, 0);
    const auto tag_begin = std::find(tag_line, str_end, '<');

    return *(tag_begin + 1) != '/';
}

std::pair<std::string, std::map<std::string, std::string>> parseTag(const char* tag_line)
{
    std::pair<std::string, std::map<std::string, std::string>> result;

    std::string tag_string(tag_line);

    const auto str_end = std::find(tag_line, tag_line + 201, 0);
    const auto tag_begin = std::find(tag_line, str_end, '<') + 1u;
    const auto tag_end = std::find(tag_begin, str_end, '>');

    const auto tag_name_range = find_word_limits(tag_begin, tag_end);
    result.first = std::string(tag_name_range.first, tag_name_range.second);

    auto attr_begin = tag_name_range.second;

    while (attr_begin < tag_end)
    {
        const auto attr_name_range = find_word_limits(attr_begin, tag_end);
        if (attr_name_range.first >= tag_end)
        {
            break;
        }

        std::string attr_name(attr_name_range.first, attr_name_range.second);

        const auto attr_value_range = find_word_limits(attr_name_range.second, tag_end);
        std::string attr_value(attr_value_range.first + 1u, attr_value_range.second - 1u);

        result.second.emplace(attr_name, attr_value);

        attr_begin = attr_value_range.second;
    }

    return result;
}

std::pair<std::vector<std::string>, std::string> parseQuery(const char* query_line)
{
    auto len = std::strlen(query_line);
    auto tilda = std::find(query_line, query_line + len, '~');

    std::vector<std::string> tag_chain;

    for (const char* word_begin = query_line; word_begin < tilda; )
    {
        const char* word_end = std::find(word_begin, tilda, '.');
        tag_chain.emplace_back(word_begin, word_end);

        if (word_end == tilda)
        {
            break;
        }

        word_begin = word_end + 1u;
    }

    return std::make_pair(tag_chain, std::string(tilda + 1u, query_line + len));
}

class TagParser
{
public:
    explicit TagParser(const char* buffer)
    {
        std::tie(result.name, result.attrs) = parseTag(buffer);
    }

    bool feed(const char* buffer)
    {
        if (child_parser)
        {
            if (child_parser->feed(buffer))
            {
                result.nested.push_back(std::move(child_parser->result));
                lines_parsed += child_parser->lines_parsed;

                delete child_parser;
                child_parser = nullptr;
            }

            return false;
        }

        if (isOpenTag(buffer))
        {
            child_parser = new TagParser(buffer);
            return false;
        }

        ++lines_parsed;

        return true;
    }

    Tag result;
    std::size_t lines_parsed = 1;

private:
    TagParser* child_parser = nullptr;
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    std::int32_t tag_count, query_count;

    std::cin >> tag_count >> query_count;

    std::map<std::string, std::map<std::string, std::string>> tag_map;

    std::vector<Tag> tags;

    TagParser* parser = nullptr;
    for (int i = 0; i < tag_count; ++i)
    {
        char buffer[201];
        std::cin.getline(buffer, 201);

        if (isTag(buffer))
        {
            if (parser)
            {
                if (parser->feed(buffer))
                {
                    tags.push_back(std::move(parser->result));
                    delete parser;
                    parser = nullptr;
                }
            }
            else
            {
                parser = new TagParser(buffer);
            }
        }
        else
        {
            --i;
        }
    }

    std::vector<std::pair<std::vector<std::string>, std::string>> queries;
    queries.reserve(query_count);

    for (int i = 0; i < query_count; ++i)
    {
        char buffer[201];
        std::cin.getline(buffer, 201);
        queries.push_back(parseQuery(buffer));
    }

    for (const auto& query : queries)
    {
        auto* tags_level = &tags;
        Tag* tag = nullptr;

        for (const auto& tag_name : query.first)
        {
            auto tag_it = std::find_if(tags_level->begin(), tags_level->end(), [&tag_name](const Tag& t){ return t.name == tag_name; });
            if (tag_it == tags_level->end())
            {
                tag = nullptr;
                std::cout << "Not Found!\n";
                break;
            }

            tag = &*tag_it;
            tags_level = &tag->nested;
        }

        if (!tag)
        {
            continue;
        }

        const auto attr_it = tag->attrs.find(query.second);
        if (attr_it != tag->attrs.end())
        {
            std::cout << attr_it->second << '\n';
        }
        else
        {
            std::cout << "Not Found!\n";
        }
    }

    return 0;
}
