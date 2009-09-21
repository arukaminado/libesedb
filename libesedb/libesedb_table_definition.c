/*
 * Table definition functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_array_type.h"
#include "libesedb_definitions.h"
#include "libesedb_table_definition.h"

/* Creates a table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_initialize(
     libesedb_table_definition_t **table_definition,
     libesedb_data_definition_t *table_data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_definition_initialize";

	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( table_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table data definition.",
		 function );

		return( -1 );
	}
	if( table_data_definition->type != LIBESEDB_DATA_DEFINITION_TYPE_TABLE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data definition type: %" PRIu16 ".",
		 function,
		 table_data_definition->type );

		return( -1 );
	}
	if( *table_definition == NULL )
	{
		*table_definition = (libesedb_table_definition_t *) memory_allocate(
		                                                     sizeof( libesedb_table_definition_t ) );

		if( *table_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create table definition.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *table_definition,
		     0,
		     sizeof( libesedb_table_definition_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear table definition.",
			 function );

			memory_free(
			 *table_definition );

			*table_definition = NULL;

			return( -1 );
		}
		if( libesedb_list_initialize(
		     &( ( *table_definition )->column_data_definition_list ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create column data definition list.",
			 function );

			memory_free(
			 *table_definition );

			*table_definition = NULL;

			return( -1 );
		}
		if( libesedb_list_initialize(
		     &( ( *table_definition )->index_data_definition_list ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index data definition list.",
			 function );

			libesedb_list_free(
			 &( ( *table_definition )->column_data_definition_list ),
			 NULL,
			 NULL );
			memory_free(
			 *table_definition );

			*table_definition = NULL;

			return( -1 );
		}
		if( libesedb_list_initialize(
		     &( ( *table_definition )->long_value_data_definition_list ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create long value data definition list.",
			 function );

			libesedb_list_free(
			 &( ( *table_definition )->index_data_definition_list ),
			 NULL,
			 NULL );
			libesedb_list_free(
			 &( ( *table_definition )->column_data_definition_list ),
			 NULL,
			 NULL );
			memory_free(
			 *table_definition );

			*table_definition = NULL;

			return( -1 );
		}
		( *table_definition )->table_data_definition = table_data_definition;
	}
	return( 1 );
}

/* Frees the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_free(
     intptr_t *table_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_definition_free";
	int result            = 1;

	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_list_free(
	     &( ( (libesedb_table_definition_t *) table_definition )->column_data_definition_list ),
	     &libesedb_data_definition_free,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free column data definition list.",
		 function );

		result = -1;
	}
	if( libesedb_list_free(
	     &( ( (libesedb_table_definition_t *) table_definition )->index_data_definition_list ),
	     &libesedb_data_definition_free,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free index data definition list.",
		 function );

		result = -1;
	}
	if( libesedb_list_free(
	     &( ( (libesedb_table_definition_t *) table_definition )->long_value_data_definition_list ),
	     &libesedb_data_definition_free,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free long value data definition list.",
		 function );

		result = -1;
	}
	memory_free(
	table_definition );

	return( result );
}

/* Appends a column data definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_append_column_data_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_data_definition_t *column_data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_definition_append_column_data_definition";

	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( column_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column data definition.",
		 function );

		return( -1 );
	}
	if( column_data_definition->type != LIBESEDB_DATA_DEFINITION_TYPE_COLUMN )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data definition type: %" PRIu16 ".",
		 function,
		 column_data_definition->type );

		return( -1 );
	}
	if( libesedb_list_append_value(
	     table_definition->column_data_definition_list,
	     (intptr_t *) column_data_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append column data definition to list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Appends an index data definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_append_index_data_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_data_definition_t *index_data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_definition_append_index_data_definition";

	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( index_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index data definition.",
		 function );

		return( -1 );
	}
	if( index_data_definition->type != LIBESEDB_DATA_DEFINITION_TYPE_INDEX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data definition type: %" PRIu16 ".",
		 function,
		 index_data_definition->type );

		return( -1 );
	}
	if( libesedb_list_append_value(
	     table_definition->index_data_definition_list,
	     (intptr_t *) index_data_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append index data definition to list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Appends a long value data definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_append_long_value_data_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_data_definition_t *long_value_data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_definition_append_long_value_data_definition";

	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( long_value_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value data definition.",
		 function );

		return( -1 );
	}
	if( long_value_data_definition->type != LIBESEDB_DATA_DEFINITION_TYPE_LONG_VALUE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data definition type: %" PRIu16 ".",
		 function,
		 long_value_data_definition->type );

		return( -1 );
	}
	if( libesedb_list_append_value(
	     table_definition->long_value_data_definition_list,
	     (intptr_t *) long_value_data_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append long value data definition to list.",
		 function );

		return( -1 );
	}
	return( 1 );
}
