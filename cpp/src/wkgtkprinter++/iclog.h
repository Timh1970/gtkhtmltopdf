#ifndef NC_ERRLOG_H
#define NC_ERRLOG_H
#include <ostream>
#include <streambuf>
#include <string>
#include <syslog.h>

#define DEBUG     true
#define DEBUG_SQL true

#define iclog_FINFO      "[File: " << __FILE_NAME__ << " Line: " << __LINE__ << "] - "
#define iclog_BREAKPOINT "[File: " << __FILE_NAME__ << " Line: " << __LINE__ << "] - (" << __PRETTY_FUNCTION__ << ")"
#define iclog_FUNCTION   "[" << __PRETTY_FUNCTION__ << "] - "

#ifndef ICLOG_API
#define ICLOG_API __attribute__((visibility("default")))
#endif

// GLOBAL DECLARATIONS FOR LOGGING

ICLOG_API extern unsigned LOG_LEVEL;

typedef ulong            BITWISE;
/**
 * @brief LOG_IGNORE
 *
 * A bitwise set of events **NOT** to log
 */
ICLOG_API extern BITWISE LOG_IGNORE;

namespace iclog {

    typedef enum {
        SQL         = 0x01LL,
        SQL_INS     = 0x02LL,
        SQL_UPD     = 0x04LL,
        SQL_SEL     = 0x08LL,
        SQL_STAT    = 0x10LL,
        IF_NCURSEES = 0x20LL,
        IF_WEB      = 0x40LL,
        DAEMON      = 0x80LL,
        CORE        = 0x100LL,
        UNDEF       = 0x200LL,
        SEC_WEB     = 0x400LL,
        SEC_NCURSES = 0x800LL,
        LIB         = 0x1000LL,
        CLI         = 0x2000LL
    } category;

    typedef enum {
        emerg    = LOG_EMERG,   // A panic condition
        alert    = LOG_ALERT,   // A condition that should be corrected
        critical = LOG_CRIT,    // Critical condition, e.g, hard device error
        error    = LOG_ERR,     // Errors
        warning  = LOG_WARNING, // Warning messages
        notice   = LOG_NOTICE,  // Possibly be handled specially
        info     = LOG_INFO,    // Informational
        debug    = LOG_DEBUG    // For debugging program
    } loglevel;

    const std::pair<category, std::string> catLUT[]{
        {SQL_SEL,     "(SQL SELECT) "         },
        {SQL_UPD,     "(SQL UPDATE) "         },
        {SQL_INS,     "(SQL INSERT) "         },
        {SQL_STAT,    "(SQL STATEMENT) "      },
        {SQL,         "(SQL) "                },
        {IF_NCURSEES, "(NCURSES INTERFACE) "  },
        {IF_WEB,      "(WEB INTERFACE) "      },
        {DAEMON,      "(DAEMON) "             },
        {CORE,        "(CORE) "               },
        {UNDEF,       "(UNDEFINED) "          },
        {SEC_WEB,     "SECURITY WEB"          },
        {SEC_NCURSES, "SECURITY NCURSES"      },
        {LIB,         "LIBRARY"               },
        {CLI,         "COMMAND LINE INTERFACE"}
    };

    const std::pair<unsigned, std::string> levelLUT[]{
        {LOG_EMERG,   "EMERGENCY: "  },
        {LOG_ALERT,   "ALERT: "      },
        {LOG_CRIT,    "CRITICAL: "   },
        {LOG_ERR,     "ERROR: "      },
        {LOG_WARNING, "WARNING: "    },
        {LOG_NOTICE,  "NOTICE: "     },
        {LOG_INFO,    "INFORMATION: "},
        {LOG_DEBUG,   "DEBUG: "      }
    };

    std::string get_level(unsigned level);
    std::string get_category(category cat);

    // Acknowledgement:  JP Embedded
    // https://github.com/jp-embedded/cpp-syslog/blob/master/src/syslog.h
    // Stremabuf; ostream; redirect (GPL) <http://www.gnu.org/licenses/>.

    //  STREAMBUF CLASS
    class streambuf : public std::streambuf {
        private:
            std::string m_buf;
            loglevel    m_level;
            int         m_category;

        public:
            streambuf();
            void level(loglevel level);
            void category(BITWISE cat);

        protected:
            int      sync();
            int_type overflow(int_type c);
    };

    // OSTREAM CLASS
    class ostream : public std::ostream {

        private:
            streambuf m_logbuf;

        public:
            ostream();
            void level(loglevel level);
            void category(BITWISE cat);
    };

    inline ostream &operator<<(ostream &os, const loglevel lev) {
        os << iclog::get_level(lev);
        os.level(lev);
        return os;
    }

    inline ostream &operator<<(ostream &os, const iclog::category cat) {
        os << iclog::get_category(cat);
        os.category(cat);
        return os;
    }

    // REDIRECT CLASS
    class redirect {
        private:
            ostream               m_dst;
            std::ostream         &m_src;
            std::streambuf *const m_sbuf;

        public:
            redirect(std::ostream &src);
            ~redirect();
    };

} // namespace iclog

ICLOG_API extern iclog::ostream jlog;

#endif // NC_ERRLOG_H
