#include "gtest/gtest.h"

#include "Url.h"
#include "SafeBuf.h"

TEST( UrlTest, SetNonAsciiValid ) {
	const char* input_urls[] = {
	    "http://topbeskæring.dk/velkommen",
	    "www.Alliancefrançaise.nu",
	    "française.Alliance.nu",
	    "française.Alliance.nu/asdf",
	    "http://française.Alliance.nu/asdf",
	    "http://française.Alliance.nu/",
	    "幸运.龍.com",
	    "幸运.龍.com/asdf/运/abc",
	    "幸运.龍.com/asdf",
	    "http://幸运.龍.com/asdf",
	    "http://Беларуская.org/Акадэмічная",
	    "https://hi.Български.com",
	    "https://fakedomain.中文.org/asdf",
	    "https://gigablast.com/abc/文/efg",
	    "https://gigablast.com/?q=文",
	    "http://www.example.сайт",
	    "http://genocidearchiverwanda.org.rw/index.php/Category:Official_Communiqués",
	    "http://www.example.com/xn--fooled-you-into-trying-to-decode-this",
	    "http://www.example.сайт/xn--fooled-you-into-trying-to-decode-this",
	    "http://腕時計通販.jp/",
	    "http://сацминэнерго.рф/robots.txt",
	    "http://faß.de/",
	    "http://βόλος.com/",
	    "http://ශ්‍රී.com/",
	    "http://نامه‌ای.com/"
	};

	const char *expected_normalized[] = {
		"http://xn--topbeskring-g9a.dk/velkommen",
		"http://www.xn--alliancefranaise-npb.nu/",
		"http://xn--franaise-v0a.alliance.nu/",
		"http://xn--franaise-v0a.alliance.nu/asdf",
		"http://xn--franaise-v0a.alliance.nu/asdf",
		"http://xn--franaise-v0a.alliance.nu/",
		"http://xn--lwt711i.xn--mi7a.com/",
		"http://xn--lwt711i.xn--mi7a.com/asdf/%E8%BF%90/abc",
		"http://xn--lwt711i.xn--mi7a.com/asdf",
		"http://xn--lwt711i.xn--mi7a.com/asdf",
		"http://xn--d0a6das0ae0bir7j.org/%D0%90%D0%BA%D0%B0%D0%B4%D1%8D%D0%BC%D1%96%D1%87%D0%BD%D0%B0%D1%8F",
		"https://hi.xn--d0a6divjd1bi0f.com/",
		"https://fakedomain.xn--fiq228c.org/asdf",
		"https://gigablast.com/abc/%E6%96%87/efg",
		"https://gigablast.com/?q=%E6%96%87",
		"http://www.example.xn--80aswg/",
		"http://genocidearchiverwanda.org.rw/index.php/Category:Official_Communiqu%C3%A9s",
		"http://www.example.com/xn--fooled-you-into-trying-to-decode-this",
		"http://www.example.xn--80aswg/xn--fooled-you-into-trying-to-decode-this",
		"http://xn--kjvp61d69f6wc3zf.jp/",
	    "http://xn--80agflthakqd0d1e.xn--p1ai/robots.txt",
		"http://xn--fa-hia.de/",
		"http://xn--nxasmm1c.com/",
		"http://xn--10cl1a0b660p.com/",
		"http://xn--mgba3gch31f060k.com/"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_normalized ) / sizeof( expected_normalized[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i] );

		EXPECT_STREQ(expected_normalized[i], (const char*)url.getUrl());
	}
}

TEST( UrlTest, SetNonAsciiInValid ) {
	const char* input_urls[] = {
		"http://www.fas.org/blog/ssp/2009/08/securing-venezuela\032s-arsenals.php",
		"https://pypi.python\n\n\t\t\t\t.org/packages/source/p/pyramid/pyramid-1.5.tar.gz",
		"http://undocs.org/ru/A/C.3/68/\vSR.48"
	};

	const char *expected_normalized[] = {
		"http://www.fas.org/blog/ssp/2009/08/securing-venezuela%1As-arsenals.php",
		"https://pypi.python/",
		"http://undocs.org/ru/A/C.3/68/%0BSR.48"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_normalized ) / sizeof( expected_normalized[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i] );

		EXPECT_STREQ(expected_normalized[i], (const char*)url.getUrl());
	}
}

TEST( UrlTest, GetDisplayUrlFromCharArray ) {
	const char* input_urls[] = {
		"http://xn--topbeskring-g9a.dk/velkommen",
		"www.xn--Alliancefranaise-npb.nu",
		"xn--franaise-v0a.Alliance.nu",
		"xn--franaise-v0a.Alliance.nu/asdf",
		"http://xn--franaise-v0a.Alliance.nu/asdf",
		"http://xn--franaise-v0a.Alliance.nu/",
		"xn--lwt711i.xn--mi7a.com",
		"xn--lwt711i.xn--mi7a.com/asdf/运/abc",
		"xn--lwt711i.xn--mi7a.com/asdf",
		"http://xn--lwt711i.xn--mi7a.com/asdf",
		"http://xn--d0a6das0ae0bir7j.org/Акадэмічная",
		"https://hi.xn--d0a6divjd1bi0f.com",
		"https://fakedomain.xn--fiq228c.org/asdf",
		"http://www.example.xn--80aswg",
		"http://www.example.com/xn--fooled-you-into-trying-to-decode-this",
		"http://www.example.xn--80aswg/xn--fooled-you-into-trying-to-decode-this",
		"http://www.example.сайт/xn--fooled-you-into-trying-to-decode-this",
		"http://xn--kjvp61d69f6wc3zf.jp/",
		"http://xn--80agflthakqd0d1e.xn--p1ai/robots.txt",
		"http://xn--80agflthakqd0d1e.xn--p1ai",
		"http://сацминэнерго.рф",
		"http://mct.verisign-grs.com/convertServlet?input=r7d.xn--g1a8ac.xn--p1ai"
	};

	const char *expected_display[] = {
		"http://topbeskæring.dk/velkommen",
		"www.Alliancefrançaise.nu",
		"française.Alliance.nu",
		"française.Alliance.nu/asdf",
		"http://française.Alliance.nu/asdf",
		"http://française.Alliance.nu/",
		"幸运.龍.com",
		"幸运.龍.com/asdf/运/abc",
		"幸运.龍.com/asdf",
		"http://幸运.龍.com/asdf",
		"http://Беларуская.org/Акадэмічная",
		"https://hi.Български.com",
		"https://fakedomain.中文.org/asdf",
		"http://www.example.сайт",
		"http://www.example.com/xn--fooled-you-into-trying-to-decode-this",
		"http://www.example.сайт/xn--fooled-you-into-trying-to-decode-this",
		"http://www.example.сайт/xn--fooled-you-into-trying-to-decode-this",
		"http://腕時計通販.jp/",
		"http://сацминэнерго.рф/robots.txt",
		"http://сацминэнерго.рф",
		"http://сацминэнерго.рф",
		"http://mct.verisign-grs.com/convertServlet?input=r7d.xn--g1a8ac.xn--p1ai"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_display ) / sizeof( expected_display[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		StackBuf( tmpBuf );
		EXPECT_STREQ( expected_display[i], (const char *) Url::getDisplayUrl( input_urls[i], &tmpBuf ));
	}
}

TEST( UrlTest, GetDisplayUrlFromUrl ) {
	const char* input_urls[] = {
		"http://topbeskæring.dk/velkommen",
		"www.Alliancefrançaise.nu",
		"française.Alliance.nu",
		"française.Alliance.nu/asdf",
		"http://française.Alliance.nu/asdf",
		"http://française.Alliance.nu/",
		"幸运.龍.com",
		"幸运.龍.com/asdf/运/abc",
		"幸运.龍.com/asdf",
		"http://幸运.龍.com/asdf",
		"http://Беларуская.org/Акадэмічная",
		"https://hi.Български.com",
		"https://fakedomain.中文.org/asdf",
		"https://gigablast.com/abc/文/efg",
		"https://gigablast.com/?q=文",
		"http://www.example.сайт",
		"http://genocidearchiverwanda.org.rw/index.php/Category:Official_Communiqués",
		"http://www.example.com/xn--fooled-you-into-trying-to-decode-this",
		"http://www.example.сайт/xn--fooled-you-into-trying-to-decode-this",
		"http://腕時計通販.jp/",
		"http://сацминэнерго.рф/robots.txt",
		"http://сацминэнерго.рф",
	    "http://mct.verisign-grs.com/convertServlet?input=r7d.xn--g1a8ac.xn--p1ai"
	};

	const char *expected_display[] = {
		"http://topbeskæring.dk/velkommen",
		"http://www.alliancefrançaise.nu/",
		"http://française.alliance.nu/",
		"http://française.alliance.nu/asdf",
		"http://française.alliance.nu/asdf",
		"http://française.alliance.nu/",
		"http://幸运.龍.com/",
		"http://幸运.龍.com/asdf/%E8%BF%90/abc",
		"http://幸运.龍.com/asdf",
		"http://幸运.龍.com/asdf",
		"http://Беларуская.org/%D0%90%D0%BA%D0%B0%D0%B4%D1%8D%D0%BC%D1%96%D1%87%D0%BD%D0%B0%D1%8F",
		"https://hi.Български.com/",
		"https://fakedomain.中文.org/asdf",
		"https://gigablast.com/abc/%E6%96%87/efg",
		"https://gigablast.com/?q=%E6%96%87",
		"http://www.example.сайт/",
		"http://genocidearchiverwanda.org.rw/index.php/Category:Official_Communiqu%C3%A9s",
		"http://www.example.com/xn--fooled-you-into-trying-to-decode-this",
		"http://www.example.сайт/xn--fooled-you-into-trying-to-decode-this",
		"http://腕時計通販.jp/",
		"http://сацминэнерго.рф/robots.txt",
		"http://сацминэнерго.рф/",
		"http://mct.verisign-grs.com/convertServlet?input=r7d.xn--g1a8ac.xn--p1ai"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_display ) / sizeof( expected_display[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i] );

		StackBuf( tmpBuf );
		EXPECT_STREQ( expected_display[i], (const char*)Url::getDisplayUrl( url.getUrl(), &tmpBuf ) );
	}
}

// make sure we don't break backward compatibility
TEST( UrlTest, StripSessionIdV122 ) {
	const char *input_urls[] = {
		"http://retailer.esignserver2.com/holzboden-direkt/gallery.do;jsessionid=D6C14EE54E6AF0B89885D129D817A505",
		"https://scholarships.wisc.edu/Scholarships/recipientDetails;jsessionid=D2DCE4F10608F15CA177E29EB2AB162F?recipId=850",
		"http://staging.ilo.org/gimi/gess/ShowProject.do;jsessionid=759cb78d694bd5a5dd5551c6eb36a1fb66b98f4e786d5ae3c73cee161067be75.e3aTbhuLbNmSe34MchaRahaRaNb0?id=1625",
		"http://ualberta.intelliresponse.com/index.jsp?requestType=NormalRequest&source=3&id=474&sessionId=f5b80817-fa7e-11e5-9343-5f3e78a954d2&question=How+many+students+are+enrolled",
		"http://www.eyecinema.ie/cinemas/film_info_detail.asp?SessionID=78C5F9DFF1B9441EB5ED527AB61BAB5B&cn=1&ci=2&ln=1&fi=7675",
		"https://jobs.bathspa.ac.uk/wrl/pages/vacancy.jsf;jsessionid=C4882E8D70D04244661C8A8E811D3290?latest=01001967",
		"https://sa.www4.irs.gov/wmar/start.do;jsessionid=DQnV2P-nFQir0foo7ThxBejZ",
		"http://www.oracle.com/technetwork/developer-tools/adf/overview/index.html;jsessionid=6R39V8WhqTQ7HMb2vTQTkzbP5XRFgs4RQzyxQ7fqxH9y6p6vKXk4!-460884186",
		"https://webshop.lic.co.nz/cws001/catalog/productDetail.jsf;jsessionid=0_cVS0dqWe1zHDcxyveGcysJVfbkUwHPDMUe_SAPzI8IIDaGbNUXn59V-PZnbFVZ;saplb_*=%28J2EE516230320%29516230351?sort=TA&wec-appid=WS001&page=B43917ED9DD446288421D9F817EE305E&itemKey=463659D75F2F005D000000000A0A0AF0&show=12&view=grid&wec-locale=en_NZ",
		"http://www.vineyard2door.com/web/clubs_browse.cfm?CFID=3843950&CFTOKEN=cfd5b9e083fb3e24-03C2F487-DAB8-1365-521658E43AB8A0DC&jsessionid=22D5211D9EB291522DE9A4258ECB94D2.cfusion",
		"http://tbinternet.ohchr.org/_layouts/treatybodyexternal/SessionDetails1.aspx?SessionID=1016&Lang=en",
		"https://collab365.conferencehosts.com/SitePages/sessionDetails.aspx?sessionid=C365117"
	};

	const char *expected_urls[] = {
		"http://retailer.esignserver2.com/holzboden-direkt/gallery.do",
		"https://scholarships.wisc.edu/Scholarships/recipientDetails?recipId=850",
		"http://staging.ilo.org/gimi/gess/ShowProject.do?id=1625",
		"http://ualberta.intelliresponse.com/index.jsp?requestType=NormalRequest&source=3&id=474&question=How+many+students+are+enrolled",
		"http://www.eyecinema.ie/cinemas/film_info_detail.asp?cn=1&ci=2&ln=1&fi=7675",
		"https://jobs.bathspa.ac.uk/wrl/pages/vacancy.jsf?latest=01001967",
		"https://sa.www4.irs.gov/wmar/start.do",
		"http://www.oracle.com/technetwork/developer-tools/adf/overview/index.html",

		// this is wrong, but it's the was it is now
		"https://webshop.lic.co.nz/cws001/catalog/productDetail.jsfsaplb_*=%28J2EE516230320%29516230351?sort=TA&wec-appid=WS001&page=B43917ED9DD446288421D9F817EE305E&itemKey=463659D75F2F005D000000000A0A0AF0&show=12&view=grid&wec-locale=en_NZ",
		"http://www.vineyard2door.com/web/clubs_browse.cfm?CFID=3843950&CFTOKEN=cfd5b9e083fb3e24-03C2F487-DAB8-1365-521658E43AB8A0DC",
		"http://tbinternet.ohchr.org/_layouts/treatybodyexternal/SessionDetails1.aspx?SessionID=1016&Lang=en",
		"https://collab365.conferencehosts.com/SitePages/sessionDetails.aspx"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 122 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, DISABLED_StripSessionIdSid ) {
	const char *input_urls[] = {
		// sid
		"http://astraklubpolska.pl/viewtopic.php?f=149&t=829138&sid=1d5e1e9ba356dc2f848f6223d914ca19&start=10",

		// sid (no strip)
		"http://www.fibsry.fi/fi/component/sobipro/?pid=146&sid=203:Bank4Hope",
	    "http://www.bzga.de/?sid=1366"
	};

	const char *expected_urls[] = {
		// sid
		"http://astraklubpolska.pl/viewtopic.php?f=149&t=829138&start=10",

		// sid (no strip)
		"http://www.fibsry.fi/fi/component/sobipro/?pid=146&sid=203:Bank4Hope",
	    "http://www.bzga.de/?sid=1366"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdPhpSessId ) {
	const char *input_urls[] = {
		// phpsessid
		"http://www.emeraldgrouppublishing.com/authors/guides/index.htm?PHPSESSID=gan1vvu81as0nnkc08fg38c3i2",
		"http://www.buf.fr/philosophy/?PHPSESSID=29ba61ff4f47064d4062e261eeab5d85",
		"http://www.toz-penkala.hr/proizvodi-skolski-pribor?phpsessid=v5bhoda67mhutnqv382q86l4l4",
		"http://web.burza.hr/?PHPSESSID",
		"http://www.dursthoff.de/book.php?PHPSESSID=068bd453c94c3c4c0b7ccca9a581597d&m=3&aid=28&bid=50",
		"http://www.sapro.cz/ftp/index.php?directory=HD-BOX&PHPSESSID=",
		"http://forum.keepemcookin.com/index.php?PHPSESSID=eoturno2s9rsrs6ru3k8j362l5&amp;action=profile;u=58995"
	};

	const char *expected_urls[] = {
		// phpsessid
		"http://www.emeraldgrouppublishing.com/authors/guides/index.htm",
		"http://www.buf.fr/philosophy/",
		"http://www.toz-penkala.hr/proizvodi-skolski-pribor",
		"http://web.burza.hr/",
		"http://www.dursthoff.de/book.php?m=3&aid=28&bid=50",
		"http://www.sapro.cz/ftp/index.php?directory=HD-BOX",
		"http://forum.keepemcookin.com/index.php?action=profile;u=58995"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdOsCommerce ) {
	const char *input_urls[] = {
		// osCommerce
		"http://www.nailcosmetics.pl/?osCAdminID=70b4c843a51204ec897136bc04282462&osCAdminID=70b4c843a51204ec897136bc04282462&osCAdminID=70b4c843a51204ec897136bc04282462&osCAdminID=70b4c843a51204ec897136bc04282462",
		"http://ezofit.sk/obchod/admin/categories.php?cPath=205&action=new_product&osCAdminID=dogjdaa5ogukr5vdtnld0o80r4",
		"http://calisonusa.com/specials.html?osCAdminID=a401c1738f8e361728c7f61e9dd23a31",
		"http://www.silversites.net/sweetheart-tree.php?osCsid=4c7154c9159ec1aadfc788a3525e61dd"
	};

	const char *expected_urls[] = {
		// osCommerce
		"http://www.nailcosmetics.pl/",
		"http://ezofit.sk/obchod/admin/categories.php?cPath=205&action=new_product",
		"http://calisonusa.com/specials.html",
		"http://www.silversites.net/sweetheart-tree.php"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdXTCommerce ) {
	const char *input_urls[] = {
		// XT-commerce
		"http://www.unitedloneliness.com/index.php/XTCsid/d929e97581813396ed8f360e7f186eab",
		"http://www.extrovert.de/Maitre?XTCsid=fgkp6js6p23gcfhl7u4g223no6",
		"https://bravisshop.eu/index.php/cPath/1/category/Professional---Hardware/XTCsid/"
	};

	const char *expected_urls[] = {
		// XT-commerce
		"http://www.unitedloneliness.com/index.php",
		"http://www.extrovert.de/Maitre",
		"https://bravisshop.eu/index.php/cPath/1/category/Professional---Hardware/"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdPostNuke ) {
	const char *input_urls[] = {
		// postnuke
		"http://eeagrants.org/News?POSTNUKESID=c9965f0db1606c402015743d1cda55f5",
		"http://www.bkamager.dk/modules.php?op=modload&name=News&file=article&sid=166&mode=thread&order=0&thold=0&POSTNUKESID=78ac739940c636f94bf9b3fac3afb4d2",
	    "http://zspieszyce.nazwa.pl/modules.php?set_albumName=pieszyce-schortens&op=modload&name=gallery&file=index&include=view_album.php&POSTNUKESID=549178d5035b622229a39cd5baf75d2a",
	    "http://myrealms.net/PostNuke/html/print.php?sid=2762&POSTNUKESID=4ed3b0a832d4687020b05ce70"
	};

	const char *expected_urls[] = {
		// postnuke
		"http://eeagrants.org/News",
		"http://www.bkamager.dk/modules.php?op=modload&name=News&file=article&sid=166&mode=thread&order=0&thold=0",
		"http://zspieszyce.nazwa.pl/modules.php?set_albumName=pieszyce-schortens&op=modload&name=gallery&file=index&include=view_album.php",
		"http://myrealms.net/PostNuke/html/print.php?sid=2762"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdColdFusion ) {
	const char *input_urls[] = {
		// coldfusion
		"http://www.vineyard2door.com/web/clubs_browse.cfm?CFID=3843950&CFTOKEN=cfd5b9e083fb3e24-03C2F487-DAB8-1365-521658E43AB8A0DC&jsessionid=22D5211D9EB291522DE9A4258ECB94D2.cfusion",
		"http://www.liquidhighwaycarwash.com/category/1118&CFID=11366594&CFTOKEN=9178789d30437e83-FD850740-F9A2-39F0-AA850FED06D46D4B/employment.htm",
	    "http://shop.arslonga.ch/index.cfm/shop/homestyle/site/article/id/16834/CFID/3458787/CFTOKEN/e718cd6cc29050df-8051DC1E-C29B-554E-6DFF6B5D2704A9A5",
		"http://www.lifeguide-augsburg.de/index.cfm/fuseaction/themen/theID/7624/ml1/7624/zg/0/cfid/43537465/cftoken/92566684.html",
	    "https://www.mutualscrew.com/cart/cart.cfm?cftokenPass=CFID%3D31481352%26CFTOKEN%3D6aac7a0fc9fa6be0%2DBF3514D1%2D155D%2D8226%2D0EF8291F836B567D%26jsessionid%3D175051907615629E4C2CB4BFC8297FF3%2Ecfusion"
	};

	const char *expected_urls[] = {
		// coldfusion
		"http://www.vineyard2door.com/web/clubs_browse.cfm",
		"http://www.liquidhighwaycarwash.com/category/1118/employment.htm",
		"http://shop.arslonga.ch/index.cfm/shop/homestyle/site/article/id/16834",
		"http://www.lifeguide-augsburg.de/index.cfm/fuseaction/themen/theID/7624/ml1/7624/zg/0",
		"https://www.mutualscrew.com/cart/cart.cfm"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdAtlassian ) {
	const char *input_urls[] = {
		// atlassian
		"https://track.systrends.com/secure/IssueNavigator.jspa?mode=show&atl_token=CUqRyjtmwj",
		"https://jira.kansalliskirjasto.fi/secure/WorkflowUIDispatcher.jspa?id=76139&action=51&atl_token=B12X-5XYK-TDON-8SC7|9724becbc02f07cdd6217c60b7662fe0b6c6f6d2|lout",
		"https://support.highwinds.com/login.action?os_destination=%2Fdisplay%2FDOCS%2FUser%2BAPI&atl_token=56c1bb338d5ad3ac262dd4e97bda482efc151f30",
	    "https://bugs.dlib.indiana.edu/secure/IssueNavigator.jspa?mode=hide&atl_token=AT3D-YZ9T-9TL1-ICW1%7C06900f3197f333cf03f196af7a36c63767c4e8fb%7Clout&requestId=10606"
	};

	const char *expected_urls[] = {
		// atlassian
		"https://track.systrends.com/secure/IssueNavigator.jspa?mode=show",
		"https://jira.kansalliskirjasto.fi/secure/WorkflowUIDispatcher.jspa?id=76139&action=51",
		"https://support.highwinds.com/login.action?os_destination=%2Fdisplay%2FDOCS%2FUser%2BAPI",
		"https://bugs.dlib.indiana.edu/secure/IssueNavigator.jspa?mode=hide&requestId=10606"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdJSessionId ) {
	const char *input_urls[] = {
		// jessionid
		"https://scholarships.wisc.edu/Scholarships/recipientDetails;jsessionid=D2DCE4F10608F15CA177E29EB2AB162F?recipId=850",
		"http://staging.ilo.org/gimi/gess/ShowProject.do;jsessionid=759cb78d694bd5a5dd5551c6eb36a1fb66b98f4e786d5ae3c73cee161067be75.e3aTbhuLbNmSe34MchaRahaRaNb0?id=1625",
		"https://jobs.bathspa.ac.uk/wrl/pages/vacancy.jsf;jsessionid=C4882E8D70D04244661C8A8E811D3290?latest=01001967",
		"https://sa.www4.irs.gov/wmar/start.do;jsessionid=DQnV2P-nFQir0foo7ThxBejZ",
		"https://webshop.lic.co.nz/cws001/catalog/productDetail.jsf;jsessionid=0_cVS0dqWe1zHDcxyveGcysJVfbkUwHPDMUe_SAPzI8IIDaGbNUXn59V-PZnbFVZ;saplb_*=%28J2EE516230320%29516230351?sort=TA&wec-appid=WS001&page=B43917ED9DD446288421D9F817EE305E&itemKey=463659D75F2F005D000000000A0A0AF0&show=12&view=grid&wec-locale=en_NZ",
		"http://www.oracle.com/technetwork/developer-tools/adf/overview/index.html;jsessionid=6R39V8WhqTQ7HMb2vTQTkzbP5XRFgs4RQzyxQ7fqxH9y6p6vKXk4!-460884186",
		"http://www.cnpas.org/portal/media-type/html/language/ro/user/anon/page/default.psml/jsessionid/A27DF3C8CF0C66C480EC74FF6A7C837C?action=forum.ScreenAction",
		"http://www.medienservice-online.de/dyn/epctrl/jsessionid/FA082288A00623E49FCC553D95D484C9/mod/wwpress002431/cat/wwpress005964/pri/wwpress"
	};

	const char *expected_urls[] = {
		// jessionid
		"https://scholarships.wisc.edu/Scholarships/recipientDetails?recipId=850",
		"http://staging.ilo.org/gimi/gess/ShowProject.do?id=1625",
		"https://jobs.bathspa.ac.uk/wrl/pages/vacancy.jsf?latest=01001967",
		"https://sa.www4.irs.gov/wmar/start.do",
		"https://webshop.lic.co.nz/cws001/catalog/productDetail.jsf?sort=TA&wec-appid=WS001&page=B43917ED9DD446288421D9F817EE305E&itemKey=463659D75F2F005D000000000A0A0AF0&show=12&view=grid&wec-locale=en_NZ",
		"http://www.oracle.com/technetwork/developer-tools/adf/overview/index.html",
		"http://www.cnpas.org/portal/media-type/html/language/ro/user/anon/page/default.psml?action=forum.ScreenAction",
		"http://www.medienservice-online.de/dyn/epctrl/mod/wwpress002431/cat/wwpress005964/pri/wwpress"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, DISABLED_StripSessionIdSessionId ) {
	const char *input_urls[] = {
		// sessionid
		"http://ualberta.intelliresponse.com/index.jsp?requestType=NormalRequest&source=3&id=474&sessionId=f5b80817-fa7e-11e5-9343-5f3e78a954d2&question=How+many+students+are+enrolled",
		"http://www.eyecinema.ie/cinemas/film_info_detail.asp?SessionID=78C5F9DFF1B9441EB5ED527AB61BAB5B&cn=1&ci=2&ln=1&fi=7675",

		// sessionid (no strip)
		"http://tbinternet.ohchr.org/_layouts/treatybodyexternal/SessionDetails1.aspx?SessionID=1016&Lang=en",
		"https://collab365.conferencehosts.com/SitePages/sessionDetails.aspx?sessionid=C365117",

		// session_id
		"https://www.insideultrasound.com/mm5/merchant.mvc?Session_ID=1f59af8e2ba36c1239ce5c897e1a90a3&Screen=PROD&Product_Code=IU400CME",
		"http://www.sylt-ferienwohnungen-urlaub.de/objekt_buchungsanfrage.php?session_id=5mt70lh8h19ci2i77h9p4e7gv5&objekt_id=644",
		"http://www.zdravotnicke-potreby.cz/main/kosik_insert.php?id_produktu=26418&session_id=gkv1ufp8spdj670c6m66qn4184&ip=",

		// session_id (no strip)
		"https://rms.miamidade.gov/Saturn/Activities/Details.aspx?session_id=53813&back_url=fi9BY3Rpdml0aWVzL1NlYXJjaC5hc3B4",
		"http://www.pbd-india.com/media-photo-gallery-event.asp?session_id=4"
	};

	const char *expected_urls[] = {
		// sessionid
		"http://ualberta.intelliresponse.com/index.jsp?requestType=NormalRequest&source=3&id=474&question=How+many+students+are+enrolled",
		"http://www.eyecinema.ie/cinemas/film_info_detail.asp?cn=1&ci=2&ln=1&fi=7675",

		// sessionid (no strip)
		"http://tbinternet.ohchr.org/_layouts/treatybodyexternal/SessionDetails1.aspx?SessionID=1016&Lang=en",
		"https://collab365.conferencehosts.com/SitePages/sessionDetails.aspx?sessionid=C365117",

		// session_id
		"https://www.insideultrasound.com/mm5/merchant.mvc?Screen=PROD&Product_Code=IU400CME",
		"http://www.sylt-ferienwohnungen-urlaub.de/objekt_buchungsanfrage.php?objekt_id=644",
		"http://www.zdravotnicke-potreby.cz/main/kosik_insert.php?id_produktu=26418&ip=",

		// session_id (no strip)
		"https://rms.miamidade.gov/Saturn/Activities/Details.aspx?session_id=53813&back_url=fi9BY3Rpdml0aWVzL1NlYXJjaC5hc3B4",
		"http://www.pbd-india.com/media-photo-gallery-event.asp?session_id=4"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		//url.print();
		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdSessId ) {
	const char *input_urls[] = {
		// vbsessid
		"https://www.westfalia.de/static/servicebereich/service/serviceangebote/impressum.html?&vbSESSID=50d96959db895a0adbfebd325a4a65e0",
		"https://www.westfalia.de/static/servicebereich/service/aktionen/banner.html?vbSESSID=f4db3ec33001c9759d095c6432651e39&cHash=5babb7ddd11f5164a9fccc7cbbf42aad",

		// asesessid
		"http://www.aseforums.com/viewtopic.php?topicid=70&asesessid=07d0b0d2dc4162ac01afe5b784940274",
		"http://hardwarelogic.com/articles.php?id=6441&asesessid=e4c6ee21fc4f3fc6f93a022647bb290b474f1c84",

		// nlsessid
		"https://www.videobuster.de/?NLSESSID=67ccc49cb2490dcb5f6d53878facf1a8",

		// sessid (no strip)
		"http://foto.ametikool.ee/index.php/oppeprotsessid/Ettev-tluserialade-reis-Saaremaa-ja-Muhu-ettev-tetesse/IMG_2216",
		"http://korel.com.au/disable-phpsessid/feed/"
	};

	const char *expected_urls[] = {
		// vbsessid
		"https://www.westfalia.de/static/servicebereich/service/serviceangebote/impressum.html",
		"https://www.westfalia.de/static/servicebereich/service/aktionen/banner.html?cHash=5babb7ddd11f5164a9fccc7cbbf42aad",

		// asesessid
		"http://www.aseforums.com/viewtopic.php?topicid=70",
		"http://hardwarelogic.com/articles.php?id=6441",

		// nlsessid
		"https://www.videobuster.de/",

		// sessid (no strip)
		"http://foto.ametikool.ee/index.php/oppeprotsessid/Ettev-tluserialade-reis-Saaremaa-ja-Muhu-ettev-tetesse/IMG_2216",
		"http://korel.com.au/disable-phpsessid/feed/"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdPSession ) {
	const char *input_urls[] = {
		// psession
		"http://ebretsteiner.at/kinderwagen/kinderwagen-Kat97.html?pSession=7d01p6qvcl2e72j8ivmppk12k0",
		"http://kontorlokaler.dk/show_unique/index.asp?Psession=491022863920110420135759"
	};

	const char *expected_urls[] = {
		// psession
		"http://ebretsteiner.at/kinderwagen/kinderwagen-Kat97.html",
		"http://kontorlokaler.dk/show_unique/index.asp"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdGalileoSession ) {
	const char *input_urls[] = {
		// galileosession
		"http://www.tutego.de/blog/javainsel/page/38/?GalileoSession=39387871A4pi84-MI8M",
		"https://shop.vierfarben.de/hilfe/Vierfarben/agb?GalileoSession=54933578A7-0S-.kn-A",
		"https://www.rheinwerk-verlag.de/?titelID=560&GalileoSession=47944076A4-xkQI91C8"
	};

	const char *expected_urls[] = {
		// galileosession
		"http://www.tutego.de/blog/javainsel/page/38/",
		"https://shop.vierfarben.de/hilfe/Vierfarben/agb",
		"https://www.rheinwerk-verlag.de/?titelID=560"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripSessionIdAuthSess ) {
	const char *input_urls[] = {
		// auth_sess
		"http://www.jobxoom.com/location.php?province=Iowa&lid=738&auth_sess=kgq6kd4bl9ma1rap6pbks1c8b2",
		"http://www.gojobcenter.com/view.php?job_id=61498&auth_sess=cb5f29174d9f5e9fbb4d7ec41cd69112&ref=bc87a09ce74326f40200e7abb"
	};

	const char *expected_urls[] = {
		// auth_sess
		"http://www.jobxoom.com/location.php?province=Iowa&lid=738",
		"http://www.gojobcenter.com/view.php?job_id=61498&ref=bc87a09ce74326f40200e7abb"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, DISABLED_StripSessionIdMySid ) {
	const char *input_urls[] = {
	    // mysid
		"https://www.worldvision.de/_downloads/allgemein/Haiti_3years_Earthquake%20Response%20Report.pdf?mysid=glwcjvci",
		"http://www.nobilia.de/file.php?mySID=80fa669565e6c41006e82e7b87b4d6c4&file=/download/Pressearchiv/Tete-A-Tete-KR%20SO14_nobilia.pdf&type=down&usg=AFQjCNELf61sLLvtPUnOJA9IwI87-ngOvQ",

		// mysid (no strip)
		"http://old.evangelskivestnik.net/statia.php?mysid=773",
		"http://www.ajusd.org/m/documents.cfm?getfiles=5170|0&mysid=1054"
	};

	const char *expected_urls[] = {
	    // mysid
		"https://www.worldvision.de/_downloads/allgemein/Haiti_3years_Earthquake%20Response%20Report.pdf",
		"http://www.nobilia.de/file.php?file=/download/Pressearchiv/Tete-A-Tete-KR%20SO14_nobilia.pdf&type=down&usg=AFQjCNELf61sLLvtPUnOJA9IwI87-ngOvQ",

		// mysid (no strip)
		"http://old.evangelskivestnik.net/statia.php?mysid=773",
		"http://www.ajusd.org/m/documents.cfm?getfiles=5170|0&mysid=1054"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, false, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

// make sure we don't break backward compatibility
TEST( UrlTest, StripTrackingParamV122 ) {
	const char *input_urls[] = {
		"http://www.urchin.com/download.html?utm_source=newsletter4&utm_medium=email&utm_term=urchin&utm_content=easter&utm_campaign=product",
		"http://www.huffingtonpost.com/parker-marie-molloy/todd-kincannon-transgender-camps_b_4100777.html?utm_source=feedburner&utm_medium=feed&utm_campaign=Feed%3A+HP%2FPolitics+%28Politics+on+The+Huffington+Post",
		"http://www.staffnet.manchester.ac.uk/news/display/?id=10341&;utm_source=newsletter&utm_medium=email&utm_campaign=eUpdate",
		"http://www.nightdivestudios.com/games/turok-dinosaur-hunter/?utm_source=steampowered.com&utm_medium=product&utm_campaign=website%20-%20turok%20dinosaur%20hunter",
		"http://www.mihomes.com/Find-Your-New-Home/Virginia-Homes?utm_source=NewHomesDirectory.com&utm_campaign=referral-division&utm_medium=feed&utm_content=&utm_term=consumer&cookiecheck=true",
		"http://www.huffingtonpost.com.au/entry/tiny-moments-happiness_us_56ec1a35e4b084c672200a36?section=australia&utm_hp_ref=healthy-living&utm_hp_ref=au-life&adsSiteOverride=au",
		"http://maersklinereefer.com/about/merry-christmas/?elqTrackId=786C9D2AE676DEC435B578D75CB0B4FD&elqaid=2666&elqat=2",
		"https://community.oracle.com/community/topliners/?elq_mid=21557&elq_cid=1618237&elq=3c0cfe27635443ca9b6410238cc876a9&elqCampaignId=2182&elqaid=21557&elqat=1&elqTrackId=40772b5725924f53bc2c6a6fb04759a3",
		"http://app.reg.techweb.com/e/er?s=2150&lid=25554&elq=00000000000000000000000000000000&elqaid=2294&elqat=2&elqTrackId=3de2badc5d7c4a748bc30253468225fd",
		"http://www.biography.com/people/louis-armstrong-9188912?elq=7fd0dd577ebf4eafa1e73431feee849f&elqCampaignId=2887",
		"http://www.thermoscientific.com/en/product/haake-mars-rotational-rheometers.html?elq_mid=1089&elq_cid=107687&wt.mc_id=CAD_MOL_MC_PR_EM1_0815_NewHaakeMars_English_GLB_2647&elq=f17d2c276ed045c0bb391e4c273b789c&elqCampaignId=&elqaid=1089&elqat=1&elqTrackId=ce2a4c4879ee4f6488a14d924fa1f8a5",
	    "https://astro-report.com/lp2.html?pk_campaign=1%20Natal%20Chart%20-%20RDMs&pk_kwd=astrological%20chart%20free&gclid=CPfkwKfP2LgCFcJc3godgSMAHA",
	    "http://lapprussia.lappgroup.com/kontakty.html?pk_campaign=yadirect-crossselling&pk_kwd=olflex&pk_source=yadirect&pk_medium=cpc&pk_content=olflex&rel=bytib"
	};

	const char *expected_urls[] = {
		"http://www.urchin.com/download.html?utm_source=newsletter4&utm_medium=email&utm_content=easter&utm_campaign=product",
		"http://www.huffingtonpost.com/parker-marie-molloy/todd-kincannon-transgender-camps_b_4100777.html?utm_medium=feed&utm_campaign=Feed%3A+HP%2FPolitics+%28Politics+on+The+Huffington+Post",
		"http://www.staffnet.manchester.ac.uk/news/display/?id=10341&utm_medium=email&utm_campaign=eUpdate",
		"http://www.nightdivestudios.com/games/turok-dinosaur-hunter/?utm_medium=product&utm_campaign=website%20-%20turok%20dinosaur%20hunter",
		"http://www.mihomes.com/Find-Your-New-Home/Virginia-Homes?utm_source=NewHomesDirectory.com&utm_campaign=referral-division&utm_medium=feed&utm_content=&cookiecheck=true",
		"http://www.huffingtonpost.com.au/entry/tiny-moments-happiness_us_56ec1a35e4b084c672200a36?section=australia&utm_hp_ref=au-life&adsSiteOverride=au",
		"http://maersklinereefer.com/about/merry-christmas/?elqTrackId=786C9D2AE676DEC435B578D75CB0B4FD&elqaid=2666&elqat=2",
		"https://community.oracle.com/community/topliners/?elq_mid=21557&elq_cid=1618237&elqCampaignId=2182&elqaid=21557&elqat=1&elqTrackId=40772b5725924f53bc2c6a6fb04759a3",
		"http://app.reg.techweb.com/e/er?s=2150&lid=25554&elqaid=2294&elqat=2&elqTrackId=3de2badc5d7c4a748bc30253468225fd",
		"http://www.biography.com/people/louis-armstrong-9188912?elqCampaignId=2887",
		"http://www.thermoscientific.com/en/product/haake-mars-rotational-rheometers.html?elq_mid=1089&elq_cid=107687&wt.mc_id=CAD_MOL_MC_PR_EM1_0815_NewHaakeMars_English_GLB_2647&elqCampaignId=&elqaid=1089&elqat=1&elqTrackId=ce2a4c4879ee4f6488a14d924fa1f8a5",
		"https://astro-report.com/lp2.html?pk_campaign=1%20Natal%20Chart%20-%20RDMs&gclid=CPfkwKfP2LgCFcJc3godgSMAHA",
		"http://lapprussia.lappgroup.com/kontakty.html?pk_campaign=yadirect-crossselling&pk_source=yadirect&pk_medium=cpc&pk_content=olflex&rel=bytib"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, false, true, 122 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripTrackingParamGoogleAnalytics ) {
	const char *input_urls[] = {
		// google analytics
		"http://www.urchin.com/download.html?utm_source=newsletter4&utm_medium=email&utm_term=urchin&utm_content=easter&utm_campaign=product",
        "http://www.huffingtonpost.com/parker-marie-molloy/todd-kincannon-transgender-camps_b_4100777.html?utm_source=feedburner&utm_medium=feed&utm_campaign=Feed%3A+HP%2FPolitics+%28Politics+on+The+Huffington+Post",
        "http://www.staffnet.manchester.ac.uk/news/display/?id=10341&;utm_source=newsletter&utm_medium=email&utm_campaign=eUpdate",
        "http://www.nightdivestudios.com/games/turok-dinosaur-hunter/?utm_source=steampowered.com&utm_medium=product&utm_campaign=website%20-%20turok%20dinosaur%20hunter",
		"http://www.mihomes.com/Find-Your-New-Home/Virginia-Homes?utm_source=NewHomesDirectory.com&utm_campaign=referral-division&utm_medium=feed&utm_content=&utm_term=consumer&cookiecheck=true",
        "http://www.huffingtonpost.com.au/entry/tiny-moments-happiness_us_56ec1a35e4b084c672200a36?section=australia&utm_hp_ref=healthy-living&utm_hp_ref=au-life&adsSiteOverride=au"
	};

	const char *expected_urls[] = {
		// google analytics
		"http://www.urchin.com/download.html",
		"http://www.huffingtonpost.com/parker-marie-molloy/todd-kincannon-transgender-camps_b_4100777.html",
		"http://www.staffnet.manchester.ac.uk/news/display/?id=10341",
		"http://www.nightdivestudios.com/games/turok-dinosaur-hunter/",
		"http://www.mihomes.com/Find-Your-New-Home/Virginia-Homes?cookiecheck=true",
		"http://www.huffingtonpost.com.au/entry/tiny-moments-happiness_us_56ec1a35e4b084c672200a36?section=australia&adsSiteOverride=au"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, false, true, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripTrackingParamOracleEloqua ) {
	const char *input_urls[] = {
        // oracle eloqua
		"http://maersklinereefer.com/about/merry-christmas/?elqTrackId=786C9D2AE676DEC435B578D75CB0B4FD&elqaid=2666&elqat=2",
        "https://community.oracle.com/community/topliners/?elq_mid=21557&elq_cid=1618237&elq=3c0cfe27635443ca9b6410238cc876a9&elqCampaignId=2182&elqaid=21557&elqat=1&elqTrackId=40772b5725924f53bc2c6a6fb04759a3",
	    "http://app.reg.techweb.com/e/er?s=2150&lid=25554&elq=00000000000000000000000000000000&elqaid=2294&elqat=2&elqTrackId=3de2badc5d7c4a748bc30253468225fd",
	    "http://www.biography.com/people/louis-armstrong-9188912?elq=7fd0dd577ebf4eafa1e73431feee849f&elqCampaignId=2887"
	};

	const char *expected_urls[] = {
		// oracle eloqua
		"http://maersklinereefer.com/about/merry-christmas/",
		"https://community.oracle.com/community/topliners/",
		"http://app.reg.techweb.com/e/er?s=2150&lid=25554",
		"http://www.biography.com/people/louis-armstrong-9188912"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, false, true, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripTrackingParamWebTrends ) {
	const char *input_urls[] = {
	    // webtrends
	    "http://www.thermoscientific.com/en/product/haake-mars-rotational-rheometers.html?elq_mid=1089&elq_cid=107687&wt.mc_id=CAD_MOL_MC_PR_EM1_0815_NewHaakeMars_English_GLB_2647&elq=f17d2c276ed045c0bb391e4c273b789c&elqCampaignId=&elqaid=1089&elqat=1&elqTrackId=ce2a4c4879ee4f6488a14d924fa1f8a5"
	};

	const char *expected_urls[] = {
		// webtrends
		"http://www.thermoscientific.com/en/product/haake-mars-rotational-rheometers.html"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, false, true, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripTrackingParamPiwik ) {
	const char *input_urls[] = {
	    // piwik
		"https://astro-report.com/lp2.html?pk_campaign=1%20Natal%20Chart%20-%20RDMs&pk_kwd=astrological%20chart%20free&gclid=CPfkwKfP2LgCFcJc3godgSMAHA",
		"http://lapprussia.lappgroup.com/kontakty.html?pk_campaign=yadirect-crossselling&pk_kwd=olflex&pk_source=yadirect&pk_medium=cpc&pk_content=olflex&rel=bytib"
	};

	const char *expected_urls[] = {
	    // piwik
		"https://astro-report.com/lp2.html",
		"http://lapprussia.lappgroup.com/kontakty.html?rel=bytib"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, false, true, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripTrackingParamTrk ) {
	const char *input_urls[] = {
	    // trk
		"https://www.nerdwallet.com/investors/?trk=nw_gn_2.0",
		"https://www.linkedin.com/company/intel-corporation?trk=ppro_cprof"
	};

	const char *expected_urls[] = {
	    // trk
		"https://www.nerdwallet.com/investors/",
		"https://www.linkedin.com/company/intel-corporation"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, false, true, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, StripTrackingParamPartnerRef ) {
	const char *input_urls[] = {
		// partnerref
		"http://www.lookfantastic.com/offers/20-off-your-top-20.list?partnerref=ENLF-_EmailExclusive"

	};

	const char *expected_urls[] = {
		// partnerref
		"http://www.lookfantastic.com/offers/20-off-your-top-20.list"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
			   sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, false, true, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}

TEST( UrlTest, Normalization ) {
	const char *input_urls[] = {
		"http://puddicatcreationsdigitaldesigns.com/index.php?route=product/product&amp;product_id=1510",
		"http://www.huffingtonpost.com.au/entry/tiny-moments-happiness_us_56ec1a35e4b084c672200a36?section=australia&adsSiteOverride=au&section=australia"
	};

	const char *expected_urls[] = {
		"http://puddicatcreationsdigitaldesigns.com/index.php?route=product/product&product_id=1510",
		"http://www.huffingtonpost.com.au/entry/tiny-moments-happiness_us_56ec1a35e4b084c672200a36?section=australia&adsSiteOverride=au"
	};

	ASSERT_EQ( sizeof( input_urls ) / sizeof( input_urls[0] ),
	           sizeof( expected_urls ) / sizeof( expected_urls[0] ) );

	size_t len = sizeof( input_urls ) / sizeof( input_urls[0] );
	for ( size_t i = 0; i < len; i++ ) {
		Url url;
		url.set( input_urls[i], strlen( input_urls[i] ), false, true, true, 123 );

		EXPECT_STREQ( expected_urls[i], (const char*)url.getUrl() );
	}
}
