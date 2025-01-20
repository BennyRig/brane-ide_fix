/* MAIN.cpp
 *   by Lut99
 *
 * Created:
 *   26 Jul 2023, 12:43:23
 * Last edited:
 *   09 Aug 2023, 13:44:31
 * Auto updated?
 *   Yes
 *
 * Description:
 *   Entrypoint for the custom Jupyter kernel for BraneScript.
 *   
 *   Based on: https://xeus.readthedocs.io/en/latest/kernel_implementation.html
**/

#include <memory>
#include "xeus/xkernel.hpp"
#include "xeus/xkernel_configuration.hpp"

#include "xeus-zmq/xserver_zmq.hpp"
#include "xeus-zmq/xzmq_context.hpp"

#include "custom_interpreter.hpp"


/***** ENTRYPOINT *****/
int main(int argc, char* argv[]) {
    // Load configuration file
    std::string file_name = (argc == 1) ? "connection.json" : argv[2];
    xeus::xconfiguration config = xeus::load_configuration(file_name);

    // Prepare Xeus' context
    //auto context = xeus::make_context<zmq::context_t>();
    auto context =  xeus::make_zmq_context();

    // Create interpreter instance
    using interpreter_ptr = std::unique_ptr<bscript::custom_interpreter>;
    interpreter_ptr interpreter = interpreter_ptr(new bscript::custom_interpreter());

    // Create kernel instance and start it
    xeus::xkernel kernel(config, xeus::get_user_name(), std::move(context), std::move(interpreter),xeus::make_xserver_default);
    kernel.start();

    return 0;
}
