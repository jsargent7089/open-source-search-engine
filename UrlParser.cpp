#include "UrlParser.h"
#include "Log.h"
#include "fctypes.h"
#include <string.h>

static const char* strnpbrk( const char *str1, size_t len, const char *str2 ) {
	const char *haystack = str1;
	const char *haystackEnd = str1 + len;

	while ( haystack < haystackEnd && *haystack ) {
		const char *needle = str2;
		while ( *needle ) {
			if ( *haystack == *needle ) {
				return haystack;
			}
			++needle;
		}
		++haystack;
	}

	return NULL;
}

UrlParser::UrlParser( const char *url, size_t urlLen )
	: m_url( url )
	, m_urlLen( urlLen )
	, m_scheme( NULL )
	, m_schemeLen( 0 )
	, m_domain( NULL )
	, m_domainLen( 0 )
	, m_paths()
	, m_queries()
	, m_queriesMap()
	, m_urlParsed() {
	m_urlParsed.reserve( m_urlLen );
	parse();
}

void UrlParser::parse() {
	// URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
	const char *urlEnd = m_url + m_urlLen;
	const char *currentPos = m_url;

	m_domain = static_cast<const char*>( memmem( currentPos, urlEnd - currentPos, "://", 3 ) );
	if ( m_domain != NULL ) {
		m_domain += 3;
		currentPos = m_domain;
	}

	const char *pathPos = static_cast<const char*>( memchr( currentPos, '/', urlEnd - currentPos ) );
	if ( pathPos != NULL ) {
		m_domainLen = pathPos - m_domain;
		currentPos = pathPos + 1;
	} else {
		m_domainLen = urlEnd - m_domain;

		// nothing else to process
		return;
	}

	const char *queryPos = pathPos ? static_cast<const char*>( memchr( currentPos, '?', urlEnd - currentPos ) ) : NULL;
	if ( queryPos != NULL ) {
		currentPos = queryPos + 1;
	}

	const char *anchorPos = pathPos ? static_cast<const char*>( memrchr( currentPos, '#', urlEnd - currentPos ) ) : NULL;
//	if ( anchorPos != NULL ) {
//		currentPos = anchorPos + 1;
//	}

	const char *pathEnd = queryPos ?: anchorPos ?: urlEnd;
	m_pathEndChar = *( pathEnd - 1 );

	const char *queryEnd = anchorPos ?: urlEnd;

	const char *prevPos = NULL;

	// path
	prevPos = pathPos + 1;
	while ( prevPos && ( prevPos <= pathEnd ) ) {
		size_t len = pathEnd - prevPos;
		currentPos = strnpbrk( prevPos, len, "/;&" );
		if ( currentPos ) {
			len = currentPos - prevPos;
		}

		UrlComponent urlPart = UrlComponent( prevPos, len, *( prevPos - 1 ) );

		m_paths.push_back( urlPart );
//		m_pathsMap[ urlPart.getKey() ] = m_paths.size() - 1;

		prevPos = currentPos ? currentPos + 1 : NULL;
	}

//	for ( std::map<std::string, size_t>::const_iterator it = m_pathsMap.begin(); it != m_pathsMap.end(); ++it ) {
//		logf(LOG_INFO, "\tpath key='%s'", it->first.c_str() );
//	}
	for ( std::vector<UrlComponent>::const_iterator it = m_paths.begin(); it != m_paths.end(); ++it ) {
		logf(LOG_TRACE, "\tseparator='%c' key='%s' path='%.*s'", it->m_separator, it->getKey().c_str(), static_cast<int32_t>( it->m_len ), it->m_pos );
	}

	// query
	if ( queryPos ) {
		prevPos = queryPos + 1;

		bool isPrevAmpersand = false;
		while ( prevPos && ( prevPos < queryEnd ) ) {
			size_t len = queryEnd - prevPos;
			currentPos = strnpbrk( prevPos, len, "&;" );
			if ( currentPos ) {
				len = currentPos - prevPos;
			}

			UrlComponent urlPart = UrlComponent( prevPos, len, *( prevPos - 1 ) );
			std::string key = urlPart.getKey();

			// check previous urlPart
			if ( isPrevAmpersand ) {
				urlPart.m_separator = '&';
			}

			bool isAmpersand = ( !urlPart.hasValue() && urlPart.getKey() == "amp" );
			if ( !key.empty() && !isAmpersand ) {
				std::map<std::string, size_t>::const_iterator it = m_queriesMap.find( key );
				if (it == m_queriesMap.end()) {
					m_queries.push_back( urlPart );
					m_queriesMap[key] = m_queries.size() - 1;
				} else {
					m_queries[it->second] = urlPart;
				}
			}

			prevPos = currentPos ? currentPos + 1 : NULL;
			isPrevAmpersand = isAmpersand;
		}

		for ( std::map<std::string, size_t>::const_iterator it = m_queriesMap.begin(); it != m_queriesMap.end(); ++it ) {
			logf(LOG_INFO, "\tqueries key='%s'", it->first.c_str() );
		}
		for ( std::vector<UrlComponent>::const_iterator it = m_queries.begin(); it != m_queries.end(); ++it ) {
			logf(LOG_TRACE, "\tseparator='%c' key='%s' query='%.*s'", it->m_separator, it->getKey().c_str(), static_cast<int32_t>( it->m_len ), it->m_pos );
		}
	}
}

const char* UrlParser::unparse() {
	m_urlParsed.clear();

	// domain
	m_urlParsed.append( m_url, ( m_domain - m_url ) + m_domainLen );

	bool isFirst = true;
	for ( std::vector<UrlComponent>::const_iterator it = m_paths.begin(); it != m_paths.end(); ++it ) {
		if ( !it->m_deleted ) {
			if ( isFirst ) {
				isFirst = false;
				if ( it->m_separator != '/' ) {
					m_urlParsed.append( "/" );
				}
			}

			m_urlParsed.append( it->m_pos - 1, it->m_len + 1 ); // include separator
		}
	}

	if ( m_urlParsed[ m_urlParsed.size() - 1 ] != '/' && m_pathEndChar == '/' ) {
		m_urlParsed += m_pathEndChar;
	}

	isFirst = true;
	for ( std::vector<UrlComponent>::const_iterator it = m_queries.begin(); it != m_queries.end(); ++it ) {
		if ( !it->m_deleted ) {
			if ( isFirst ) {
				isFirst = false;
				m_urlParsed.append( "?" );
			} else {
				m_urlParsed += ( it->m_separator == '?' ) ? '&' : it->m_separator;
			}

			m_urlParsed.append( it->m_pos, it->m_len );
		}
	}

	if ( m_urlLen != m_urlParsed.size() ) {
		logf( LOG_INFO, "@@@ in =%.*s", static_cast<int32_t>( m_urlLen ), m_url );
		logf( LOG_INFO, "@@@ out=%s", m_urlParsed.c_str());
	}

	return m_urlParsed.c_str();
}

bool UrlParser::removeComponent( const std::vector<UrlComponent*> &urlComponents, const UrlComponent::Validator &validator ) {
	bool hasRemoval = false;

	for ( std::vector<UrlComponent*>::const_iterator it = urlComponents.begin(); it != urlComponents.end(); ++it ) {
		if ( (*it)->m_deleted ) {
			continue;
		}

		if ( ( (*it)->hasValue() && validator.isValid( *(*it) ) ) ||
		     ( !(*it)->hasValue() && validator.allowEmptyValue() ) ) {
			hasRemoval = true;
			(*it)->m_deleted = true;
		}
	}

	return hasRemoval;
}

std::vector<std::pair<UrlComponent*, UrlComponent*> > UrlParser::matchPath( const UrlComponent::Matcher &matcher ) {
	std::vector<std::pair<UrlComponent*, UrlComponent*> > result;

	for ( std::vector<UrlComponent>::iterator it = m_paths.begin(); it != m_paths.end(); ++it ) {
		if ( it->m_deleted ) {
			continue;
		}

		if ( !it->hasValue() && matcher.isMatching( *it ) ) {
			std::vector<UrlComponent>::iterator valueIt = it;
			std::advance( valueIt, 1 ); /// @todo change to std::next (c++11)

			result.push_back( std::make_pair( &( *it ), ( valueIt != m_paths.end() ? &( *valueIt ) : NULL ) ) );
		}
	}

	return result;
}

bool UrlParser::removePath( const std::vector<std::pair<UrlComponent*, UrlComponent*> > &urlComponents,
                            const UrlComponent::Validator &validator ) {
	bool hasRemoval = false;
	std::vector<std::pair<UrlComponent*, UrlComponent*> >::const_iterator it;
	for ( it = urlComponents.begin(); it != urlComponents.end(); ++it ) {
		if ( it->second == NULL ) {
			if ( validator.allowEmptyValue() ) {
				hasRemoval = true;
				it->first->m_deleted = true;
			}
		} else {
			if ( validator.isValid( *( it->second ) ) ) {
				hasRemoval = true;
				it->first->m_deleted = true;
				it->second->m_deleted = true;
			}
		}
	}

	return hasRemoval;
}

bool UrlParser::removePath( const UrlComponent::Matcher &matcher, const UrlComponent::Validator &validator ) {
	std::vector<std::pair<UrlComponent*, UrlComponent*> > matches = matchPath( matcher );

	return removePath( matches, validator );
}

std::vector<UrlComponent*> UrlParser::matchPathParam( const UrlComponent::Matcher &matcher ) {
	std::vector<UrlComponent*> result;

	for ( std::vector<UrlComponent>::iterator it = m_paths.begin(); it != m_paths.end(); ++it ) {
		if ( it->m_deleted ) {
			continue;
		}

		if ( it->hasValue() && matcher.isMatching( *it ) ) {
			result.push_back( &( *it ) );
		}
	}

	return result;
}

bool UrlParser::removePathParam( const std::vector<UrlComponent*> &urlComponents, const UrlComponent::Validator &validator ) {
	return removeComponent( urlComponents, validator );
}

bool UrlParser::removePathParam( const UrlComponent::Matcher &matcher, const UrlComponent::Validator &validator ) {
	std::vector<UrlComponent*> matches = matchPathParam( matcher );

	return removeComponent( matches, validator );
}

std::vector<UrlComponent*> UrlParser::matchQueryParam( const UrlComponent::Matcher &matcher ) {
	std::vector<UrlComponent*> result;

	for ( std::vector<UrlComponent>::iterator it = m_queries.begin(); it != m_queries.end(); ++it ) {
		if ( it->m_deleted ) {
			continue;
		}

		if ( matcher.isMatching( *it ) ) {
			result.push_back( &( *it ) );
		}
	}

	return result;
}

bool UrlParser::removeQueryParam( const char *param ) {
	static const UrlComponent::Validator s_validator( 0, 0, true, ALLOW_ALL, MANDATORY_NONE );
	UrlComponent::Matcher matcher( param );

	return removeQueryParam( matcher, s_validator );
}

bool UrlParser::removeQueryParam( const std::vector<UrlComponent*> &urlComponents, const UrlComponent::Validator &validator ) {
	return removeComponent( urlComponents, validator );
}

bool UrlParser::removeQueryParam( const UrlComponent::Matcher &matcher, const UrlComponent::Validator &validator ) {
	std::vector<UrlComponent*> matches = matchQueryParam( matcher );

	return removeComponent( matches, validator );
}
