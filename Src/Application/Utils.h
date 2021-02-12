//
// Created by u7i on 2/12/21.
//

#ifndef PVC_UTILS_H
#define PVC_UTILS_H

#include <QString>

namespace PVC{
    /**
     * \brief Small utils functions that used in application ( not in core )
     */
    class Utils{
    public:
        /**
         * \brief Select the longest string from two
         * @param first First string
         * @param second Second string
         * @return The longest string
         */
        static QString longest(const QString &first, const QString &second){
            return (first.length() > second.length()) ? first : second;
        }
    };
}

#endif //PVC_UTILS_H
