# wkgtkprinter++

A lightweight C++ library for generating PDFs from HTML using WebKit2GTK.

## Key Features

- **Modern & Secure**: Uses WebKit2GTK (not deprecated wkhtmltopdf)
- **Tiny Footprint**: ~60KB binary, minimal dependencies
- **Image Support**: Base64 encodes and inlines images automatically
- **Smart CSS**: Built-in responsive layout system with visual margin guides
- **Easy to Use**: Simple API with comprehensive examples

## Why Use This?

Most HTML-to-PDF tools struggle with:
- Margins and page sizing
- Image handling
- Consistent rendering across devices

Our solution fixes these by:
1. Using CSS variables to control layout
2. Providing visual debugging with border indicators
3. Automatically handling common PDF rendering gotchas

## Quick Start

### Command Line Interface

For quick conversions without writing code:

```bash
html2pdf -i infile.html -o outfile.pdf
```

### cpp
```
#include <wkgtkprinter++/ichtmltopdf++.h>
#include <wkgtkprinter++/pretty_html.h>

int main() {
    icGTK::init();
    WEBPAGE html("<!DOCTYPE html>");
    html_tree dom("html", html);
    
    // Create page structure
    html_tree *body = dom.new_node("body");
    html_tree *page = body->new_node("div class=\"page\"");
    html_tree *subpage = page->new_node("div class=\"subpage\"");

    // Add content
    subpage->new_node("h1")->set_node_content("Hello World");
    
    // Process and generate PDF
    pretty_html::process_nodes(&dom);
    PDFprinter pdf;
    pdf.set_param(html, "/usr/share/wkgtkprinter/a4-portrait-pdf.page", "/tmp/output.pdf");
    pdf.make_pdf();
    
    return 0;
}
```
> **Note**: The PDF will be generated in the current working directory. 
> Use an absolute path if you need output in a specific location.



