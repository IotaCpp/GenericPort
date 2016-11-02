/* === DONE === */

#include "Storage.h"
#include "Node.h"
#include "TipsManager.h"
#include "API.h"
#include "Snapshot.h"
#include <cstdio>       // fprintf()
#include <cstdlib>      // EXIT_SUCCESS, EXIT_FAILURE, atexit()
#include <exception>


constexpr const char *NAME = "IRI";
constexpr const char *VERSION = "1.1.0";

static void shutdown (void)
{
    API::shutDown();
    TipsManager::shutDown();
    Node::shutDown();
    Storage::shutDown();
}


int main (int argc, char *argv[])
{
    fprintf( stdout, "%s %s\n", NAME, VERSION );
    atexit( shutdown );

    int exitCode = EXIT_SUCCESS;
    try
    {
        Converter::initStatics();
        Snapshot::initStatics();

        Storage::launch();
        Node::launch( argc, argv );
        TipsManager::launch();
        API::launch();
    }
    catch (const std::exception &e)
    {
        fprintf( stderr, "internal error: %s.\n", e.what() );
        exitCode = EXIT_FAILURE;
    }
    catch (...)
    {
        fprintf( stderr, "internal error: unknown exception thrown.\n" );
        exitCode = EXIT_FAILURE;
    }

    return exitCode;
}
