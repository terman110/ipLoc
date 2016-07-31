#include "menuicon.h"
#include "dlg.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QApplication>
#include <QMap>
#include <QDirIterator>

menuicon::menuicon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    timerval = QSettings().value("timer value", 10000).toInt();
    m_lastIP = "";

    fillCountryMap();

    re = new QRegularExpression("<p class=\"([\\w]+)\">(.*)</p>", QRegularExpression::CaseInsensitiveOption | QRegularExpression::InvertedGreedinessOption);

    QDirIterator it(":/flagIcons/flagIcons", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QFileInfo fi(it.next());
        lstIcons.append(fi.baseName());
    }
    this->setIcon(QIcon(":/flagIcons/flagIcons/unknown.png"));

    menu = new QMenu("ipLoc");
    a_loc = menu->addAction( "--");
    a_loc->setEnabled(false);
    a_ip = menu->addAction( "xx.xx.xx.xx");
    a_ip->setEnabled(false);
    a_date = menu->addAction( "Last updated: never");
    a_date->setEnabled(false);
    QAction* a_space = menu->addAction("");
    a_space->setEnabled(false);
    QAction* a_dlg = menu->addAction( "Settings");
    connect(a_dlg, SIGNAL(triggered()), this, SLOT(onDlg()));
    a_restart = menu->addAction("Restart");
    connect(a_restart, SIGNAL(triggered()), this, SLOT(onRestart()));
    a_close = menu->addAction("Quit");
    connect(a_close, SIGNAL(triggered()), this, SLOT(onClose()));
    this->setContextMenu( menu);
    timer = new QTimer(this);
    timer->setInterval(timerval);
    timer->start(timerval);;
    timer->setTimerType(Qt::VeryCoarseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(idle()));

    sourceUrl = "http://lightgraffiti.de/ip.php";
    m_source = new FileDownloader(sourceUrl, this);
    connect(m_source, SIGNAL(downloaded()), SLOT(loadSource()));

    idle();
}
void menuicon::fillCountryMap()
{
    dictCountry.insert("AD", "Andorra");
    dictCountry.insert("AE", "United Arab Emirates");
    dictCountry.insert("AF", "Afghanistan");
    dictCountry.insert("AG", "Antigua and Barbuda");
    dictCountry.insert("AI", "Anguilla");
    dictCountry.insert("AL", "Albania");
    dictCountry.insert("AM", "Armenia");
    dictCountry.insert("AO", "Angola");
    dictCountry.insert("AQ", "Antarctica");
    dictCountry.insert("AR", "Argentina");
    dictCountry.insert("AS", "American Samoa");
    dictCountry.insert("AT", "Austria");
    dictCountry.insert("AU", "Australia");
    dictCountry.insert("AW", "Aruba");
    dictCountry.insert("AX", "Åland Islands");
    dictCountry.insert("AZ", "Azerbaijan");
    dictCountry.insert("BA", "Bosnia and Herzegovina");
    dictCountry.insert("BB", "Barbados");
    dictCountry.insert("BD", "Bangladesh");
    dictCountry.insert("BE", "Belgium");
    dictCountry.insert("BF", "Burkina Faso");
    dictCountry.insert("BG", "Bulgaria");
    dictCountry.insert("BH", "Bahrain");
    dictCountry.insert("BI", "Burundi");
    dictCountry.insert("BJ", "Benin");
    dictCountry.insert("BL", "Saint Barthélemy");
    dictCountry.insert("BM", "Bermuda");
    dictCountry.insert("BN", "Brunei Darussalam");
    dictCountry.insert("BO", "Bolivia, Plurinational State of");
    dictCountry.insert("BQ", "Bonaire, Sint Eustatius and Saba");
    dictCountry.insert("BR", "Brazil");
    dictCountry.insert("BS", "Bahamas");
    dictCountry.insert("BT", "Bhutan");
    dictCountry.insert("BV", "Bouvet Island");
    dictCountry.insert("BW", "Botswana");
    dictCountry.insert("BY", "Belarus");
    dictCountry.insert("BZ", "Belize");
    dictCountry.insert("CA", "Canada");
    dictCountry.insert("CC", "Cocos (Keeling) Islands");
    dictCountry.insert("CD", "Congo, the Democratic Republic of the");
    dictCountry.insert("CF", "Central African Republic");
    dictCountry.insert("CG", "Congo");
    dictCountry.insert("CH", "Switzerland");
    dictCountry.insert("CI", "Côte d'Ivoire");
    dictCountry.insert("CK", "Cook Islands");
    dictCountry.insert("CL", "Chile");
    dictCountry.insert("CM", "Cameroon");
    dictCountry.insert("CN", "China");
    dictCountry.insert("CO", "Colombia");
    dictCountry.insert("CR", "Costa Rica");
    dictCountry.insert("CU", "Cuba");
    dictCountry.insert("CV", "Cabo Verde");
    dictCountry.insert("CW", "Curaçao");
    dictCountry.insert("CX", "Christmas Island");
    dictCountry.insert("CY", "Cyprus");
    dictCountry.insert("CZ", "Czech Republic");
    dictCountry.insert("DE", "Germany");
    dictCountry.insert("DJ", "Djibouti");
    dictCountry.insert("DK", "Denmark");
    dictCountry.insert("DM", "Dominica");
    dictCountry.insert("DO", "Dominican Republic");
    dictCountry.insert("DZ", "Algeria");
    dictCountry.insert("EC", "Ecuador");
    dictCountry.insert("EE", "Estonia");
    dictCountry.insert("EG", "Egypt");
    dictCountry.insert("EH", "Western Sahara");
    dictCountry.insert("ER", "Eritrea");
    dictCountry.insert("ES", "Spain");
    dictCountry.insert("ET", "Ethiopia");
    dictCountry.insert("FI", "Finland");
    dictCountry.insert("FJ", "Fiji");
    dictCountry.insert("FK", "Falkland Islands (Malvinas)");
    dictCountry.insert("FM", "Micronesia, Federated States of");
    dictCountry.insert("FO", "Faroe Islands");
    dictCountry.insert("FR", "France");
    dictCountry.insert("GA", "Gabon");
    dictCountry.insert("GD", "Grenada");
    dictCountry.insert("GE", "Georgia");
    dictCountry.insert("GF", "French Guiana");
    dictCountry.insert("GG", "Guernsey");
    dictCountry.insert("GH", "Ghana");
    dictCountry.insert("GI", "Gibraltar");
    dictCountry.insert("GL", "Greenland");
    dictCountry.insert("GM", "Gambia");
    dictCountry.insert("GN", "Guinea");
    dictCountry.insert("GP", "Guadeloupe");
    dictCountry.insert("GQ", "Equatorial Guinea");
    dictCountry.insert("GR", "Greece");
    dictCountry.insert("GS", "South Georgia and the South Sandwich Islands");
    dictCountry.insert("GT", "Guatemala");
    dictCountry.insert("GU", "Guam");
    dictCountry.insert("GW", "Guinea-Bissau");
    dictCountry.insert("GY", "Guyana");
    dictCountry.insert("HK", "Hong Kong");
    dictCountry.insert("HM", "Heard Island and McDonald Islands");
    dictCountry.insert("HN", "Honduras");
    dictCountry.insert("HR", "Croatia");
    dictCountry.insert("HT", "Haiti");
    dictCountry.insert("HU", "Hungary");
    dictCountry.insert("ID", "Indonesia");
    dictCountry.insert("IE", "Ireland");
    dictCountry.insert("IL", "Israel");
    dictCountry.insert("IM", "Isle of Man");
    dictCountry.insert("IN", "India");
    dictCountry.insert("IO", "British Indian Ocean Territory");
    dictCountry.insert("IQ", "Iraq");
    dictCountry.insert("IR", "Iran, Islamic Republic of");
    dictCountry.insert("IS", "Iceland");
    dictCountry.insert("IT", "Italy");
    dictCountry.insert("JE", "Jersey");
    dictCountry.insert("JM", "Jamaica");
    dictCountry.insert("JO", "Jordan");
    dictCountry.insert("JP", "Japan");
    dictCountry.insert("KE", "Kenya");
    dictCountry.insert("KG", "Kyrgyzstan");
    dictCountry.insert("KH", "Cambodia");
    dictCountry.insert("KI", "Kiribati");
    dictCountry.insert("KM", "Comoros");
    dictCountry.insert("KN", "Saint Kitts and Nevis");
    dictCountry.insert("KP", "Korea, Democratic People's Republic of");
    dictCountry.insert("KR", "Korea, Republic of");
    dictCountry.insert("KW", "Kuwait");
    dictCountry.insert("KY", "Cayman Islands");
    dictCountry.insert("KZ", "Kazakhstan");
    dictCountry.insert("LA", "Lao People's Democratic Republic");
    dictCountry.insert("LB", "Lebanon");
    dictCountry.insert("LC", "Saint Lucia");
    dictCountry.insert("LI", "Liechtenstein");
    dictCountry.insert("LK", "Sri Lanka");
    dictCountry.insert("LR", "Liberia");
    dictCountry.insert("LS", "Lesotho");
    dictCountry.insert("LT", "Lithuania");
    dictCountry.insert("LU", "Luxembourg");
    dictCountry.insert("LV", "Latvia");
    dictCountry.insert("LY", "Libya");
    dictCountry.insert("MA", "Morocco");
    dictCountry.insert("MC", "Monaco");
    dictCountry.insert("MD", "Moldova, Republic of");
    dictCountry.insert("ME", "Montenegro");
    dictCountry.insert("MF", "Saint Martin (French part)");
    dictCountry.insert("MG", "Madagascar");
    dictCountry.insert("MH", "Marshall Islands");
    dictCountry.insert("MK", "Macedonia, the former Yugoslav Republic of");
    dictCountry.insert("ML", "Mali");
    dictCountry.insert("MM", "Myanmar");
    dictCountry.insert("MN", "Mongolia");
    dictCountry.insert("MO", "Macao");
    dictCountry.insert("MP", "Northern Mariana Islands");
    dictCountry.insert("MQ", "Martinique");
    dictCountry.insert("MR", "Mauritania");
    dictCountry.insert("MS", "Montserrat");
    dictCountry.insert("MT", "Malta");
    dictCountry.insert("MU", "Mauritius");
    dictCountry.insert("MV", "Maldives");
    dictCountry.insert("MW", "Malawi");
    dictCountry.insert("MX", "Mexico");
    dictCountry.insert("MY", "Malaysia");
    dictCountry.insert("MZ", "Mozambique");
    dictCountry.insert("NA", "Namibia");
    dictCountry.insert("NC", "New Caledonia");
    dictCountry.insert("NE", "Niger");
    dictCountry.insert("NF", "Norfolk Island");
    dictCountry.insert("NG", "Nigeria");
    dictCountry.insert("NI", "Nicaragua");
    dictCountry.insert("NL", "Netherlands");
    dictCountry.insert("NO", "Norway");
    dictCountry.insert("NP", "Nepal");
    dictCountry.insert("NR", "Nauru");
    dictCountry.insert("NU", "Niue");
    dictCountry.insert("NZ", "New Zealand");
    dictCountry.insert("OM", "Oman");
    dictCountry.insert("PA", "Panama");
    dictCountry.insert("PE", "Peru");
    dictCountry.insert("PF", "French Polynesia");
    dictCountry.insert("PG", "Papua New Guinea");
    dictCountry.insert("PH", "Philippines");
    dictCountry.insert("PK", "Pakistan");
    dictCountry.insert("PL", "Poland");
    dictCountry.insert("PM", "Saint Pierre and Miquelon");
    dictCountry.insert("PN", "Pitcairn");
    dictCountry.insert("PR", "Puerto Rico");
    dictCountry.insert("PS", "Palestine, State of");
    dictCountry.insert("PT", "Portugal");
    dictCountry.insert("PW", "Palau");
    dictCountry.insert("PY", "Paraguay");
    dictCountry.insert("QA", "Qatar");
    dictCountry.insert("RE", "Réunion");
    dictCountry.insert("RO", "Romania");
    dictCountry.insert("RS", "Serbia");
    dictCountry.insert("RU", "Russian Federation");
    dictCountry.insert("RW", "Rwanda");
    dictCountry.insert("SA", "Saudi Arabia");
    dictCountry.insert("SB", "Solomon Islands");
    dictCountry.insert("SC", "Seychelles");
    dictCountry.insert("SD", "Sudan");
    dictCountry.insert("SE", "Sweden");
    dictCountry.insert("SG", "Singapore");
    dictCountry.insert("SH", "Saint Helena, Ascension and Tristan da Cunha");
    dictCountry.insert("SI", "Slovenia");
    dictCountry.insert("SJ", "Svalbard and Jan Mayen");
    dictCountry.insert("SK", "Slovakia");
    dictCountry.insert("SL", "Sierra Leone");
    dictCountry.insert("SM", "San Marino");
    dictCountry.insert("SN", "Senegal");
    dictCountry.insert("SO", "Somalia");
    dictCountry.insert("SR", "Suriname");
    dictCountry.insert("SS", "South Sudan");
    dictCountry.insert("ST", "Sao Tome and Principe");
    dictCountry.insert("SV", "El Salvador");
    dictCountry.insert("SX", "Sint Maarten (Dutch part)");
    dictCountry.insert("SY", "Syrian Arab Republic");
    dictCountry.insert("SZ", "Swaziland");
    dictCountry.insert("TC", "Turks and Caicos Islands");
    dictCountry.insert("TD", "Chad");
    dictCountry.insert("TF", "French Southern Territories");
    dictCountry.insert("TG", "Togo");
    dictCountry.insert("TH", "Thailand");
    dictCountry.insert("TJ", "Tajikistan");
    dictCountry.insert("TK", "Tokelau");
    dictCountry.insert("TL", "Timor-Leste");
    dictCountry.insert("TM", "Turkmenistan");
    dictCountry.insert("TN", "Tunisia");
    dictCountry.insert("TO", "Tonga");
    dictCountry.insert("TR", "Turkey");
    dictCountry.insert("TT", "Trinidad and Tobago");
    dictCountry.insert("TV", "Tuvalu");
    dictCountry.insert("TW", "Taiwan, Province of China");
    dictCountry.insert("TZ", "Tanzania, United Republic of");
    dictCountry.insert("UA", "Ukraine");
    dictCountry.insert("UG", "Uganda");
    dictCountry.insert("UM", "United States Minor Outlying Islands");
    dictCountry.insert("US", "United States of America");
    dictCountry.insert("UY", "Uruguay");
    dictCountry.insert("UZ", "Uzbekistan");
    dictCountry.insert("VA", "Holy See");
    dictCountry.insert("VC", "Saint Vincent and the Grenadines");
    dictCountry.insert("VE", "Venezuela, Bolivarian Republic of");
    dictCountry.insert("VG", "Virgin Islands, British");
    dictCountry.insert("VI", "Virgin Islands, U.S.");
    dictCountry.insert("VN", "Viet Nam");
    dictCountry.insert("VU", "Vanuatu");
    dictCountry.insert("WF", "Wallis and Futuna");
    dictCountry.insert("WS", "Samoa");
    dictCountry.insert("YE", "Yemen");
    dictCountry.insert("YT", "Mayotte");
    dictCountry.insert("ZA", "South Africa");
    dictCountry.insert("ZM", "Zambia");
    dictCountry.insert("ZW", "Zimbabwe");
}

void menuicon::loadImage(QString countryCode)
{
    try
    {
        countryCode = countryCode.toLower();
        if(lstIcons.contains(countryCode))
            this->setIcon(QIcon(":/flagIcons/flagIcons/" + countryCode + ".png"));
        else
            this->setIcon(QIcon(":/flagIcons/flagIcons/unknown.png"));
        this->show();
    }
    catch (std::exception e) { }
}

void menuicon::loadSource()
{
    try
    {
        QString src = m_source->downloadedString();
        //m_source->setUrl(sourceUrl);

        QMap<QString, QString> dict;

        QRegularExpressionMatchIterator i = re->globalMatch(src);
        QRegularExpressionMatch match;
        while (i.hasNext()) {
            match = i.next();
            QString key = match.captured(1).toLower();
            QString val = match.captured(2);
            if(!key.isEmpty() && !val.isEmpty())
                dict.insert(key, val);
        }

        if(!dict.contains("ip"))
            return;

        QString ip = dict["ip"];
        if(ip == m_lastIP)
            return;

        a_ip->setText(ip);
        m_lastIP = ip;

        if(dict.contains("country"))
        {
            QString country = dict["country"];
            loadImage(country);
            if(dictCountry.contains(country.toUpper()))
                country = dictCountry[country.toUpper()];
            a_loc->setText(country);
        }
        else
        {
            loadImage("");
            a_loc->setText("Unknown");
        }

        QTime time = QTime::currentTime();
        a_date->setText("Last updated: " + time.toString());
    }
    catch (std::exception e) { }
}

void menuicon::idle()
{
    try
    {
        m_source->setUrl(sourceUrl);
    }
    catch (std::exception e) { }
}

void menuicon::onDlg()
{
    timer->stop();
    dlg* m_dlg = new dlg( &timerval);
    m_dlg->setModal( true);
    m_dlg->show();
    m_dlg->exec();

    QSettings().setValue("timer value", timerval);

    onRestart();
}

void menuicon::onClose()
{
    QSettings().setValue("timer value", timerval);
    timer->stop();
    QApplication::quit();
}

void menuicon::onRestart()
{
    timer->stop();
    timer->setInterval(timerval);
    timer->start(timerval);
    idle();
}
