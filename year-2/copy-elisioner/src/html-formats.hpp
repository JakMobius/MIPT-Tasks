#pragma once

#include <string>
#include <ostream>

template<typename T>
struct QuotedPrinter {
    const T& m_value;

    QuotedPrinter(const T& value): m_value(value) {}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const QuotedPrinter<T>& printer) {
    stream << "\"" << printer.m_value << "\"";
    return stream;
}

template<typename T>
struct BlockCommentPrinter {
    const T& m_value;

    BlockCommentPrinter(const T& value): m_value(value) {}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const BlockCommentPrinter<T>& printer) {
    stream << "/* " << printer.m_value << " */";
    return stream;
}

template<typename T>
struct HTMLColoredPrinter {
    const T& m_value;
    const std::string& m_color;

    HTMLColoredPrinter(const T& value, const std::string& color): m_value(value), m_color(color) {}
};

template<typename T>
struct HTMLSpanColoredPrinter : public HTMLColoredPrinter<T> {
    HTMLSpanColoredPrinter(const T& value, const std::string& color): HTMLColoredPrinter<T>(value, color) {}
};

template<typename T>
struct HTMLFontColoredPrinter: public HTMLColoredPrinter<T> {
    HTMLFontColoredPrinter(const T& value, const std::string& color): HTMLColoredPrinter<T>(value, color) {}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const HTMLSpanColoredPrinter<T>& printer) {
    stream << "<span style='color:" << printer.m_color << "'>" << printer.m_value << "</span>";
    return stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const HTMLFontColoredPrinter<T>& printer) {
    stream << "<font color=" << QuotedPrinter(printer.m_color) << ">" << printer.m_value << " </font>";
    return stream;
}