#ifndef GTKPRINT_H
#define GTKPRINT_H
#include <condition_variable>
#include <mutex>
#include <string>
#include <systemd/sd-bus.h>
#include <thread>
#include <webkit2/webkit2.h>

#ifndef PDF_API
#define PDF_API __attribute__((visibility("default")))
#endif

/**
 * @brief The html2pdf_params class
 *
 * A definition payload that is passed to the function that creates the pdf
 */
struct html2pdf_params {
        const char              *in_uri;
        const char              *html_txt;
        const char              *base_uri;
        const char              *out_uri;
        const char              *key_file_data;
        const char              *default_stylesheet;
        std::mutex              *wait_mutex;
        std::condition_variable *wait_cond;
        int                     *wait_data;
        ;
};

/**
 * @brief The XvfbMode enum
 *
 * Modes to run xvfb in.
 *
 * @note START_STOP may be used in some edge cases, but is only
 * really intened for testing.  The recommended way to run the
 * aplicaiton is to leave xvfb running.
 *
 * @note xvfb Is only required on headless servers, it will not be started
 * unless it is needed.
 *
 */
enum class XvfbMode {
    START_STOP,   // Start on demand, stop on exit
    KEEP_RUNNING, // Start on demand, keep running
};

class WKGTK_init {
    private:
        GMainLoop              *loop = nullptr;
        std::thread             glob_Thread;
        std::mutex              init_mutex;
        std::condition_variable init_cond;

    public:
        WKGTK_init();
        ~WKGTK_init();
};

/**
 * @brief The icGTK class
 * Test and set up the environment and create an instance of WKGTK_init
 * if all is good.
 *
 * @note this is a singleton class.
 */
class icGTK {

    private:
        WKGTK_init tk;
        XvfbMode   runMode;

        WKGTK_init  handle_xvfb_daemon();
        std::string check_xvfb(sd_bus *bus, const std::string &service);
        bool        start_service(sd_bus *bus);
        bool        stop_service(sd_bus *bus);

        icGTK(XvfbMode runMode);
        ~icGTK();

    public:
        PDF_API static icGTK &init(XvfbMode runMode = XvfbMode::KEEP_RUNNING);
        PDF_API               icGTK(const icGTK &)     = delete;
        PDF_API icGTK        &operator=(const icGTK &) = delete;
};

/**
 * @brief The PDFprinter class
 *
 * This class is to make the pdf.
 *
 * It expects an instance of icGTK to be running before it is called
 *
 */
class PDF_API PDFprinter {
    private:
        char *in_uri;
        char *html_txt;
        char *base_uri;
        char *out_uri;
        char *key_file_data;
        char *default_stylesheet;

        /**
         * @brief payload
         * Payload only contains pointers to the values, not the
         * values themselves.
         */
        html2pdf_params payload;

    public:
        PDFprinter();
        ~PDFprinter();
        void set_param(std::string html, std::string printSettings, std::string outFile);
        void make_pdf();

        static std::string read_file(const std::string &fullPath);
};

#endif // GTKPRINT_H
