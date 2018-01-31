#ifndef __LOGGING_HPP__
#define __LOGGING_HPP__

/**
 * Defines the preprocessor macro IF_DEBUG(command) which executes command only
 * in debug mode. This can be used to suppress log messages when not in
 * debug mode.
 *
 * @example IF_DEBUG(std::cerr << "my custom debug log message" << std::endl;)
 * @link https://stackoverflow.com/a/14251257/1220835
 */
#if ISO_LIB_ENABLE_DEBUG_OUTPUT
# define IF_DEBUG(x) x
#else
# define IF_DEBUG(x)
#endif

/**
 * Defines a preprocessor variable that can be used as a string stream. This
 * enables us to suppress log messages when not in debug mode.
 *
 * @example DEBUG << "my custom debug log message" << std::endl;
 *
 * In most optimization levels this method will also result in the preprocessor
 * removing the whole output command, meaning that any commands that are part
 * of the message generation will not be executed. In the following example
 * the method foo() will likely not be invoked, because the compiler sees the
 * empty block behind "if(1)" and removes the whole statement. If optimization
 * is disabled, foo() will be invoked.
 *
 * @example DEBUG << foo() << std::endl;
 * @link https://stackoverflow.com/a/11826787/1220835
 */
#if ISO_LIB_ENABLE_DEBUG_OUTPUT
# define DEBUG \
if (0) {} \
else std::cerr
#else
# define DEBUG \
if (1) {} \
else std::cerr
#endif

#endif // __LOGGING_HPP__
