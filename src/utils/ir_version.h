#ifndef __IR_VERSION_H__
#define __IR_VERSION_H__

/**
 * @brief Version of the intermediate representation written by argoc and read by argov.
 *
 * The first line of an IR file is "ARGONAUT_IR <version>". Bump the number whenever the tables
 * or the node types change, so that argov refuses an IR produced by another compiler version
 * instead of misreading it.
 */
#define ARGONAUT_IR_VERSION 1

#endif // __IR_VERSION_H__
