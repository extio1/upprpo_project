#include "document.h"
#include "glyphs/page.h"
#include "glyphs/glyph.h"

#include <algorithm>
#include <cassert>

Document::Document() {
    currentPage = std::make_shared<Page>(0, 0, pageWidth, pageHeight);
    pages.push_back(currentPage);
    visiblePages.push_back(currentPage);
}

void Document::SetCurrentPage(PagePtr page) {
    currentPage = std::move(page);
}

Document::PagePtr Document::GetCurrentPage() {
    return currentPage;
}

size_t Document::GetPageCount() const {
    return pages.size();
}

size_t Document::GetPageHeight() const {
    return pageHeight;
}

void Document::AddPage(const Glyph::GlyphPtr& page) {
    pages.push_back(page);
}

Document::PagePtr Document::GetNextPage(const Page* page) {
    auto currentPage = std::find_if(pages.begin(), pages.end(), [&](const auto& elem) {
        return elem.get() == page;
    });

    assert(currentPage != pages.end());

    auto nextPage = std::next(currentPage);
    if(nextPage == pages.end()) {
        return nullptr;
    }
    return std::static_pointer_cast<Page>(*nextPage);
}