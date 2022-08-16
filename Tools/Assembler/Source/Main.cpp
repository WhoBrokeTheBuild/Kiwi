
#include <Kiwi/Utility/Math.hpp>
#include <Kiwi/Utility/String.hpp>
#include <Kiwi/Utility/Exception.hpp>

#include <cflags.hpp>

using namespace kiwi;

#include <fstream>
#include <regex>
#include <optional>

class MultiBufferIterator
{
public:

    ~MultiBufferIterator()
    {
        for (char * buffer : _bufferList) {
            delete buffer;
        }
    }

    void append_buffer(char * buffer)
    {
        _bufferList.push_back(buffer);
        _viewList.push_back(buffer);
    }

    void insert_buffer(char * buffer, unsigned replace = 0)
    {
        _bufferList.push_back(buffer);

        StringView view = _viewList[_viewIndex];
        _viewList.erase(_viewList.begin() + _viewIndex);

        _viewList.insert(_viewList.begin() + _viewIndex, view.substr(_dataIndex + replace));
        _viewList.insert(_viewList.begin() + _viewIndex, buffer);
        _viewList.insert(_viewList.begin() + _viewIndex, view.substr(0, _dataIndex));

        _dataIndex = 0;
        ++_viewIndex;
    }

    void reset()
    {
        _viewIndex = 0;
        _dataIndex = 0;
    }

    char peek(int offset = 0)
    {
        size_t tmpDataIndex = _dataIndex;
        size_t tmpViewIndex = _viewIndex;

        if (_viewList.empty()) {
            return '\0';
        }

        while (tmpDataIndex + offset >= _viewList[tmpViewIndex].size()) {
            offset -= _viewList[tmpViewIndex].size() - tmpDataIndex;
            ++tmpViewIndex;
            tmpDataIndex = 0;

            if (tmpViewIndex >= _viewList.size()) {
                return '\0';
            }
        }

        return _viewList[tmpViewIndex][tmpDataIndex + offset];
    }

    char pop()
    {
        if (_viewList.empty()) {
            return '\0';
        }

        if (_dataIndex + 1 > _viewList[_viewIndex].size()) {
            ++_viewIndex;
            _dataIndex = 0;

            if (_viewIndex >= _viewList.size()) {
                return '\0';
            }
        }

        char c = _viewList[_viewIndex][_dataIndex];
        ++_dataIndex;
        return c;
    }

    size_t pop_until(char terminator, char * buffer, size_t limit)
    {
        if (limit == 0) {
            return 0;
        }

        size_t bufferIndex = 0;
        
        if (_viewList.empty()) {
            return 0;
        }

        while (_viewIndex < _viewList.size()) {
            if (_dataIndex >= _viewList[_viewIndex].size()) {
                ++_viewIndex;
                _dataIndex = 0;
            }
            else {
                char c = _viewList[_viewIndex][_dataIndex];
                ++_dataIndex;

                buffer[bufferIndex] = c;
                ++bufferIndex;

                if (c == terminator || bufferIndex >= limit - 1) {
                    break;
                }
            }
        }

        buffer[bufferIndex] = '\0';
        return bufferIndex;
    }

    void skip(int amount = 1)
    {
        if (_viewList.empty()) {
            return;
        }

        while (_dataIndex + amount > _viewList[_viewIndex].size()) {
            amount -= _viewList[_viewIndex].size() - _dataIndex;
            ++_viewIndex;
            _dataIndex = 0;

            if (_viewIndex >= _viewList.size()) {
                return;
            }
        }

        _dataIndex += amount;
    }

private:

    List<char *> _bufferList;

    List<StringView> _viewList;

    size_t _viewIndex = 0;

    size_t _dataIndex = 0;

};

void parse(StringView filename)
{
    auto file = std::ifstream(filename.data());
    if (!file) {
        return;
    }

    MultiBufferIterator it;
    it.append_buffer(new char[]{
        "Hello, World\n"
        "#include <ass>\n"
        "Or whatever\n"
    });

    it.skip(13);
    it.insert_buffer(new char[]{
        "what do you think\n"
        "about this\n"
    }, 15);

    it.reset();
    char line[100];
    for (;;) {
        size_t bytesRead = it.pop_until('\n', line, sizeof(line));
        if (bytesRead == 0) {
            break;
        }

        printf("%s", line);
    }

}

int main(int argc, char * argv[])
{
    cflags::cflags flags;

    int status = 0;

    bool help = false;
    flags.add_bool('h', "help", &help, "Display help information and exit.");

    String language = "auto";
    flags.add_string('\0', "language", &language, "Assembly language/instruction set to use. Default is 'auto'.");

    String output = "a.o";
    flags.add_string('o', "output", &output, "Output filename.");

    flags.parse(argc, argv);

    if (flags.argc == 0) {
        status = 1;
        fprintf(stderr, "No sources specified.\n");
    }

    if (help || status != 0) {
        flags.print_usage(
            "[OPTION]... SOURCE...",
            "Assembler for the Kiwi Emulator Suite",
            "Additional information about this tool can be found by at:\n"
            "  https://github.com/WhoBrokeTheBuild/Kiwi"
        );

        return status;
    }

    for (auto filename : flags.args) {
        parse(filename);
    }

    return 0;
}