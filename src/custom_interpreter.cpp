/* CUSTOM INTERPRETER.cpp
 *   by Lut99
 *
 * Created:
 *   13 Jun 2023, 17:39:03
 * Last edited:
 *   29 Jun 2023, 13:48:24
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Implements the custom interpreter for BraneScript.
 *   
 *   Based on: https://xeus.readthedocs.io/en/latest/kernel_implementation.html
**/

#include <string>
#include <unordered_map>
#include <iostream>

#include "brane/brane_tsk.h"
#include "custom_interpreter.hpp"

using namespace std;


/***** CONSTANTS *****/
/// The kernel version.
const static char* KERNEL_VERSION = "1.0.0";





/***** GLOBALS *****/
/// Whether we are in a usable state or not.
bool errored = false;
/// The map of dynamically loaded compiler functions.
Functions* functions;
/// The global hashmap of sessions to running compilers.
unordered_map<string, Compiler*> sessions;





/***** LIBRARY *****/
/* Essentially a "start" function that is executed after Kernel creation, and before any request is executed. */
void custom_interpreter::configure_impl() override {
    // Let's only log for now
    cout << "Initializing BraneScript kernel v" << KERNEL_VERSION << "..." << endl;

    // Load the dynamic functions
    functions = functions_load("/libbrane_tsk.so");
    if (functions == NULL) {
        // We cannot continue!
        cerr << "Initialization errored." << endl;
        errored = true;
        return;
    }

    // Initialize the sessions map
    sessions = unordered_map();

    // Done
    cout << "Initialization done." << endl;
}

/* Essentially a "stop" function that is executed before the kernel is shutdown completely. */
void shutdown_request_impl() {
    // Only do stuff if not errorred
    if (errored) { return; }
    cout << "Terminating BraneScript kernel v" << KERNEL_VERSION << "..." << endl;

    // Clean the globals
    for (pair<string, Compiler*> : sessions) {
        functions->compiler_free(pair.second);
    }
    functions_unload(functions);

    // Done
    cout << "Termination complete." << endl;
}



/* The main course of this class.
 * 
 * This function handles the execution of a BraneScript snippet, and effectively implements the interpreter.
 * 
 * # Arguments
 * - `execution_counter`: The number of times a cell has already been executed.
 * - `code`: The raw string snippet that we need to compile and execute.
 * - `silent`: If true, then the execution of this request should have as little effects as possible. Typically, this means no broadcasts on any IOPUB channels or execute_results.
 * - `store_history`: Whether to populate history or not. TODO: It's unclear whether this means any state updates?
 * - `user_expressions`: This can be used to acquire any prompt(?) information from the user.
 * - `allow_stdin`: Whether the kernel can query the user for some stdin input (i.e., mid-execution input).
 * 
 * # Returns
 * JSON signalling either a successfull execution or a failing execution. Generated by either `xeus::create_successful_reply()` or `create_error_reply()`.
 */
nl::json custom_interpreter::execute_request_impl(int execution_counter, const std::string& code, bool silent, bool store_history, nl::json user_expressions, bool allow_stdin) override {
    cout << "Handling execute request " << execution_counter << endl;

    // Fetch the Rust code.
}
