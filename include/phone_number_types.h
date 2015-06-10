/*
 * Phonenumber Utils
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_TYPES_H__
#define __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_TYPES_H__


/**
 * @file phone_number_types.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_PHONE_NUMBER_UTILS_MODULE
 *
 * @{
 */

/**
 * @brief Enumeration for language type.
 *
 * @since_tizen 2.4
 *
 */
typedef enum {
	PHONE_NUMBER_LANG_AMHARIC,                      /**< Amharic */
	PHONE_NUMBER_LANG_ARABIC,                       /**< Arabic */
	PHONE_NUMBER_LANG_BELARUSIAN,                   /**< Belarusian */
	PHONE_NUMBER_LANG_BULGARIAN,                    /**< Bulgarian */
	PHONE_NUMBER_LANG_CHINESE,                      /**< Chinese */
	PHONE_NUMBER_LANG_CHINESE_TRADITIONAL,          /**< Chinese(Traditional) */
	PHONE_NUMBER_LANG_CZECH,                        /**< Czech */
	PHONE_NUMBER_LANG_DUTCH,                        /**< Dutch */
	PHONE_NUMBER_LANG_ENGLISH,                      /**< English */
	PHONE_NUMBER_LANG_FINNISH,                      /**< Finnish */
	PHONE_NUMBER_LANG_FRENCH,                       /**< French */
	PHONE_NUMBER_LANG_GERMAN,                       /**< German */
	PHONE_NUMBER_LANG_GREEK,                        /**< Greek */
	PHONE_NUMBER_LANG_HUNGARIAN,                    /**< Hungarian */
	PHONE_NUMBER_LANG_INDONESIAN,                   /**< Indonesian */
	PHONE_NUMBER_LANG_ITALIAN,                      /**< Italian */
	PHONE_NUMBER_LANG_JAPANESE,                     /**< Japanese */
	PHONE_NUMBER_LANG_KOREAN,                       /**< Korean */
	PHONE_NUMBER_LANG_NORTHERN_SAMI,                /**< Northern Sami */
	PHONE_NUMBER_LANG_PERSIAN,                      /**< Persian */
	PHONE_NUMBER_LANG_POLISH,                       /**< Polish */
	PHONE_NUMBER_LANG_PORTUGUESE,                   /**< Portuguese */
	PHONE_NUMBER_LANG_ROMANIAN,                     /**< Romanian */
	PHONE_NUMBER_LANG_RUSSIAN,                      /**< Russian */
	PHONE_NUMBER_LANG_SERBIAN,                      /**< Serbian */
	PHONE_NUMBER_LANG_SPANISH,                      /**< Spanish */
	PHONE_NUMBER_LANG_SWEDISH,                      /**< Swedish */
	PHONE_NUMBER_LANG_THAI,                         /**< Thai */
	PHONE_NUMBER_LANG_TURKISH,                      /**< Turkish */
	PHONE_NUMBER_LANG_VIETNAMESE,                   /**< Vietnamese */
	PHONE_NUMBER_LANG_MAX,
} phone_number_lang_e;

/**
 * @brief Enumeration for region type.
 *
 * @since_tizen 2.4
 *
 */
typedef enum {
	PHONE_NUMBER_REGION_ALAND_ISLANDS,                              /**< Aland Islands */
	PHONE_NUMBER_REGION_ALBANIA,                                    /**< Albania */
	PHONE_NUMBER_REGION_ALGERIA,                                    /**< Algeria */
	PHONE_NUMBER_REGION_AMERICAN_SAMOA,                             /**< American Samoa */
	PHONE_NUMBER_REGION_ANGOLA,                                     /**< Angola */
	PHONE_NUMBER_REGION_ANGUILLA,                                   /**< Anguilla */
	PHONE_NUMBER_REGION_ARGENTINA,                                  /**< Argentina */
	PHONE_NUMBER_REGION_ARMENIA,                                    /**< Armenia */
	PHONE_NUMBER_REGION_ASCENSION_ISLAND,                           /**< Ascension Island */
	PHONE_NUMBER_REGION_ATIGUA_AND_BARBUDA,                         /**< Atigua and Barbuda */
	PHONE_NUMBER_REGION_AUSTRIA,                                    /**< Austria */
	PHONE_NUMBER_REGION_BAHAMAS,                                    /**< Bahamas */
	PHONE_NUMBER_REGION_BARBADOS,                                   /**< Barbados */
	PHONE_NUMBER_REGION_BELARUS,                                    /**< Belarus */
	PHONE_NUMBER_REGION_BELGIUM,                                    /**< Belgium */
	PHONE_NUMBER_REGION_BENIN,                                      /**< Benin */
	PHONE_NUMBER_REGION_BERMUDA,                                    /**< Bermuda */
	PHONE_NUMBER_REGION_BONAIRE_SINT_EUSTATIUS_AND_SABA,            /**< Bonaire Sint Eustatius and Saba */
	PHONE_NUMBER_REGION_BOTSWANA,                                   /**< Botswana */
	PHONE_NUMBER_REGION_BRAZIL,                                     /**< Brazil */
	PHONE_NUMBER_REGION_BULGARIA,                                   /**< Bulgaria */
	PHONE_NUMBER_REGION_BURKINA_FASO,                               /**< Burkina Faso */
	PHONE_NUMBER_REGION_BURUNDI,                                    /**< Burundi */
	PHONE_NUMBER_REGION_CABO_VERDE,                                 /**< Cabo Verde */
	PHONE_NUMBER_REGION_CAMEROON,                                   /**< Cameroon */
	PHONE_NUMBER_REGION_CANADA,                                     /**< Canada */
	PHONE_NUMBER_REGION_CAYMAN_ISLAND,                              /**< Cayman Island */
	PHONE_NUMBER_REGION_CHILE,                                      /**< Chile */
	PHONE_NUMBER_REGION_CHINA,                                      /**< China */
	PHONE_NUMBER_REGION_COLOMBIA,                                   /**< Colombia */
	PHONE_NUMBER_REGION_COMOROS,                                    /**< Comoros */
	PHONE_NUMBER_REGION_CONGO,                                      /**< Congo */
	PHONE_NUMBER_REGION_COTE_D_IVOIRE,                              /**< Cote d'Ivoire */
	PHONE_NUMBER_REGION_CUBA,                                       /**< Cuba */
	PHONE_NUMBER_REGION_CURACAO,                                    /**< Curacao */
	PHONE_NUMBER_REGION_CZECH_REPUBLIC,                             /**< Czech Republic */
	PHONE_NUMBER_REGION_DEMOCRATIC_REPUBLIC_OF_THE_CONGO,           /**< Congo (Democratic Republic of the) */
	PHONE_NUMBER_REGION_DOMINICA,                                   /**< Dominica */
	PHONE_NUMBER_REGION_DOMINICAN_REPUBLIC,                         /**< Domoinican Republic */
	PHONE_NUMBER_REGION_EGYPT,                                      /**< Egypt */
	PHONE_NUMBER_REGION_ESTONIA,                                    /**< Estonia */
	PHONE_NUMBER_REGION_ETHIOPIA,                                   /**< Ethiopia */
	PHONE_NUMBER_REGION_FINLAND,                                    /**< Finland */
	PHONE_NUMBER_REGION_FRANCE,                                     /**< France */
	PHONE_NUMBER_REGION_GABON,                                      /**< Gabon */
	PHONE_NUMBER_REGION_GAMBIA,                                     /**< Gambia */
	PHONE_NUMBER_REGION_GERMANY,                                    /**< Germany */
	PHONE_NUMBER_REGION_GHANA,                                      /**< Ghana */
	PHONE_NUMBER_REGION_GREECE,                                     /**< Greece */
	PHONE_NUMBER_REGION_GREENLAND,                                  /**< Greenland */
	PHONE_NUMBER_REGION_GRENADA,                                    /**< Grenada */
	PHONE_NUMBER_REGION_GUAM,                                       /**< Guam */
	PHONE_NUMBER_REGION_GUERNSEY,                                   /**< Guernsey */
	PHONE_NUMBER_REGION_GUINEA,                                     /**< Guinea */
	PHONE_NUMBER_REGION_GUINEA_BISSAU,                              /**< Guinea-Bissau */
	PHONE_NUMBER_REGION_HUNGARY,                                    /**< Hungary */
	PHONE_NUMBER_REGION_ICELAND,                                    /**< Iceland */
	PHONE_NUMBER_REGION_INDIA,                                      /**< India */
	PHONE_NUMBER_REGION_INDONESIA,                                  /**< Indonesia */
	PHONE_NUMBER_REGION_IRELAND,                                    /**< Ireland */
	PHONE_NUMBER_REGION_ISLAMIC_REPUBLIC_OF_IRAN,                   /**< Iran (Islamic Republic of) */
	PHONE_NUMBER_REGION_ISLE_OF_MAN,                                /**< Isle of Man */
	PHONE_NUMBER_REGION_ITALY,                                      /**< Italy */
	PHONE_NUMBER_REGION_JAMAICA,                                    /**< Jamaica */
	PHONE_NUMBER_REGION_JAPAN,                                      /**< Japan */
	PHONE_NUMBER_REGION_JERSEY,                                     /**< Jersey */
	PHONE_NUMBER_REGION_JORDAN,                                     /**< Jordan */
	PHONE_NUMBER_REGION_KAZAKHSTAN,                                 /**< Kazakhstan */
	PHONE_NUMBER_REGION_KENYA,                                      /**< Kenya */
	PHONE_NUMBER_REGION_KIRIBATI,                                   /**< Kiribati */
	PHONE_NUMBER_REGION_LATVIA,                                     /**< Latvia */
	PHONE_NUMBER_REGION_LESOTHO,                                    /**< Lesotho */
	PHONE_NUMBER_REGION_LITHUANIA,                                  /**< Lithuania */
	PHONE_NUMBER_REGION_LUXEMBOURG,                                 /**< Luxembourg */
	PHONE_NUMBER_REGION_MADAGASCAR,                                 /**< Madagascar */
	PHONE_NUMBER_REGION_MAURITANIA,                                 /**< Mauritania */
	PHONE_NUMBER_REGION_MAURITIUS,                                  /**< Maritius */
	PHONE_NUMBER_REGION_MONTSERRAT,                                 /**< Montserrat */
	PHONE_NUMBER_REGION_MOROCCO,                                    /**< Morocco */
	PHONE_NUMBER_REGION_MOZAMBIQUE,                                 /**< Mazambique */
	PHONE_NUMBER_REGION_NAMIBIA,                                    /**< Namibia */
	PHONE_NUMBER_REGION_NETHERLANDS,                                /**< Netherlands */
	PHONE_NUMBER_REGION_NIGERIA,                                    /**< Nigeria */
	PHONE_NUMBER_REGION_NORTHERN_MARIANA_ISLANDS,                   /**< Northern Mariana Island */
	PHONE_NUMBER_REGION_NORWAY,                                     /**< Norway */
	PHONE_NUMBER_REGION_PERU,                                       /**< Peru */
	PHONE_NUMBER_REGION_POLAND,                                     /**< Poland */
	PHONE_NUMBER_REGION_PORTUGAL,                                   /**< Portugal */
	PHONE_NUMBER_REGION_PUERTO_RICO,                                /**< Puerto Rico */
	PHONE_NUMBER_REGION_REPUBLIC_OF_KOREA,                          /**< Korea (Republic of) */
	PHONE_NUMBER_REGION_REPUBLIC_OF_MOLDOVA,                        /**< Moldova (Republic of)  */
	PHONE_NUMBER_REGION_ROMANIA,                                    /**< Romania */
	PHONE_NUMBER_REGION_RUSSIAN_FEDERATION,                         /**< Russian Federation */
	PHONE_NUMBER_REGION_SAINT_HELENA,                               /**< Saint Helena, Ascension and Tristan da Cunha */
	PHONE_NUMBER_REGION_SAINT_KITTS_AND_NEVIS,                      /**< Saint Kitts and Nevis */
	PHONE_NUMBER_REGION_SAINT_LUCIA,                                /**< Saint Lucia */
	PHONE_NUMBER_REGION_SAINT_VINCENT_AND_THE_GRENADINES,           /**< Saint Vincent and the Grenadines */
	PHONE_NUMBER_REGION_SAO_TOME_AND_PRINCIPE,                      /**< Sao Tome and Principe */
	PHONE_NUMBER_REGION_SAUDI_ARABIA,                               /**< Saudi Arabia */
	PHONE_NUMBER_REGION_SENEGAL,                                    /**< Senegal */
	PHONE_NUMBER_REGION_SERBIA,                                     /**< Serbia */
	PHONE_NUMBER_REGION_SIERRA_LEONE,                               /**< Sierra Leone */
	PHONE_NUMBER_REGION_SINT_MAARTEN,                               /**< Sint maarten (Dutch part) */
	PHONE_NUMBER_REGION_SLOVAKIA,                                   /**< Slovakia */
	PHONE_NUMBER_REGION_SOMALIA,                                    /**< Somalia */
	PHONE_NUMBER_REGION_SOUTH_AFRICA,                               /**< South Africa */
	PHONE_NUMBER_REGION_SPAIN,                                      /**< Spain */
	PHONE_NUMBER_REGION_SRI_LANKA,                                  /**< Sri Lanka */
	PHONE_NUMBER_REGION_SUDAN,                                      /**< Sudan */
	PHONE_NUMBER_REGION_SVALBARD_AND_JAN_MAYEN,                     /**< Svalbard and Jan Mayen */
	PHONE_NUMBER_REGION_SWAZILAND,                                  /**< Swaziland */
	PHONE_NUMBER_REGION_SWEDEN,                                     /**< Sweden */
	PHONE_NUMBER_REGION_SWITZERLAND,                                /**< Switzerland */
	PHONE_NUMBER_REGION_TAIWAN_PROVINCE_OF_CHINA,                   /**< Taiwan Province of China */
	PHONE_NUMBER_REGION_THAILAND,                                   /**< Thailand */
	PHONE_NUMBER_REGION_THE_FORMER_YUGOSLAV_REPUBLIC_OF_MACEDONIA,  /**< Macedonia (the Former Yugoslav Republic of) */
	PHONE_NUMBER_REGION_TOGO,                                       /**< Togo */
	PHONE_NUMBER_REGION_TRINIDAD_AND_TOBAGO,                        /**< Trinidad and Tobago */
	PHONE_NUMBER_REGION_TRISTAN_DA_CUNHA,                           /**< Tristan da Cunha */
	PHONE_NUMBER_REGION_TUNISIA,                                    /**< Tunisia */
	PHONE_NUMBER_REGION_TURKEY,                                     /**< Turkey */
	PHONE_NUMBER_REGION_TURKS_AND_CAICOS_ISLANDS,                   /**< Turks and Caicos Islands */
	PHONE_NUMBER_REGION_UGANDA,                                     /**< Uganda */
	PHONE_NUMBER_REGION_UNITED_KINGDOM,                             /**< United Kingdom */
	PHONE_NUMBER_REGION_UNITED_STATES_OF_AMERICA,                   /**< United States of America */
	PHONE_NUMBER_REGION_VENEZUELA,                                  /**< Venezuela */
	PHONE_NUMBER_REGION_VIET_NAM,                                   /**< Viet Nam */
	PHONE_NUMBER_REGION_VIRGIN_ISLAND_BRITISH,                      /**< Virgin Island (British) */
	PHONE_NUMBER_REGION_VIRGIN_ISLAND_US,                           /**< Virgin Island (U.S.) */
	PHONE_NUMBER_REGION_WESTERN_SAHARA,                             /**< Western Sahara */
	PHONE_NUMBER_REGION_ZAMBIA,                                     /**< Zambia */
	PHONE_NUMBER_REGION_ZIMBABWE,                                   /**< Zimbabwe */
	PHONE_NUMBER_REGION_MAX,
} phone_number_region_e;


/**
 * @}
 */

#endif /* __TIZEN_TELEPHONY_PHONE_NUMBER_UTILS_TYPES_H__ */
