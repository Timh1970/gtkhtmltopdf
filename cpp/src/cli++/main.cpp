#include <curses.h>
#include <filesystem>
#include <iostream>
#include <systemd/sd-journal.h>
#include <wkgtkprinter++/ichtmltopdf++.h>
#include <wkgtkprinter++/iclog.h>

using std::string;

/**
 * @brief main
 * @return
 *
 * This is a test of the inplicare print library version.
 */
int main(int argc, char *argv[]) {

    // INITIALISE WEBKIT2GTK
    icGTK::init();

    // SET UP LOGGING (all logging to journal)
    LOG_LEVEL = LOG_DEBUG;

    // REDIRECT WEBKIT LOGGING TO SYSLOG
    dup2(sd_journal_stream_fd(argv[0], LOG_LEVEL, 1), STDERR_FILENO);
    setlogmask(LOG_UPTO(LOG_LEVEL));
    setlocale(LC_CTYPE, "en_GB.UTF-8");

    // HANDLE COMMAND LINE ARGUMENTS
    int    opt = 0;
    string infile;
    string outfile;
    string css;
    string path;
    while ((opt = getopt(argc, argv, "i:o:c:p:")) != -1) {
        switch (opt) {
            case 'i': // CONFIG FILE
                infile = std::filesystem::current_path().string() + "/" + string(optarg);
                break;
            case 'o': // LOG LEVEL
                outfile = std::filesystem::current_path().string() + "/" + string(optarg);
                break;
            case 'c': // CSS FILE (more effort than it is worth; buggy on padding)
                css = string(optarg);
                break;
            case 'p': // PATH
                path = string(optarg);
                break;
            default:
                break;
        }
    }

    if (infile.empty() || outfile.empty()) {
        std::cout << "USAGE: " << argv[0] << " -i infile.html -o outfile.pdf\n"
                  << std::endl;
        exit(0);
    }

    PDFprinter pdf;
    pdf.set_param(
        PDFprinter::read_file(infile),
        PDFprinter::read_file("/usr/share/icprint/a4-portrait-pdf.page"),
        outfile
    );
    pdf.make_pdf();

    return (0);
}
