/* CUSTOM INTERPRETER.hpp
 *   by Lut99
 *
 * Created:
 *   13 Jun 2023, 16:09:11
 * Last edited:
 *   14 Jun 2023, 09:57:53
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Header file for the custom interpreter implementation for BraneScript.
 * 
 *   Based on: https://xeus.readthedocs.io/en/latest/kernel_implementation.html
**/

#ifndef CUSTOM_INTERPRETER_HPP
#define CUSTOM_INTERPRETER_HPP

#include <xeus/xinterpreter.hpp>
#include "nlohmann/json.hpp"

using xeus::xinterpreter;
namespace nl = nlohmann;

namespace bscript {
    class custom_interpreter: public xinterpreter {
    public:
        /* (Default) Constructor for the `custom_interpreter`. */
        custom_interpreter() = default;
        /* (Default) Destructor for the `custom_interpreter`. */
        virtual ~custom_interpreter() = default;


    private:
        /* Essentially a "start" function that is executed after Kernel creation, and before any request is executed. */
        void configure_impl() override;

        /* Essentially a "stop" function that is executed before the kernel is shutdown completely. */
        void shutdown_request_impl();



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
        nl::json execute_request_impl(int execution_counter, const std::string& code, bool silent, bool store_history, nl::json user_expressions, bool allow_stdin) override;

        /* Provides auto-complete support based on the current token.
         * 
         * # Arguments
         * - `code`: The token/snippet to autocomplete.
         * - `cursor_pos`: The position of the cursor in this `code`.
         * 
         * # Returns
         * JSON describing what to autocomplete. Generated by `xeus::create_complete_reply()`.
         */
        nl::json complete_request_impl(const std::string& code, int cursor_pos);

        /* Allows the user to inspect some token in the code.
         * 
         * Note that, because Jupyter is in the dark on this one, token extraction is up to us.
         * 
         * # Arguments
         * - `code`: The snippet to inspect.
         * - `cursor_pos`: The position of the cursor in this `code`.
         * 
         * # Returns
         * JSON describing the inspected token. Generated by `xeus::create_inspect_reply()`.
         */
        nl::json inspect_request_impl(const std::string& code, int cursor_pos);

        /* Can be used by the frontend to query if a snippet of code is a complete snippet (i.e., an entire statement).
         * 
         * # Arguments
         * - `code`: The snippet to deduce the completeness(?) off.
         * 
         * # Returns
         * JSON describing whether it was complete or nah. Generated by `xeus::create_is_complete_reply()`.
         */
         nl::json is_complete_request_impl(const std::string& code);

        /* Returns metadata about the kernel itself.
         * 
         * # Returns
         * JSON describing the kernel. Generated by `xeus::create_info_reply()`.
         */
         nl::json kernel_info_request_impl();
    };
}

#endif
