//
//  ESDObject.hpp
//  EyeOfSauronDHTMonitor
//
//  Created by shuaizhai on 1/23/16.
//  Copyright © 2016 com.dhtMonitor.www. All rights reserved.
//

#ifndef ESDObject_hpp
#define ESDObject_hpp

#define CREATE_ERROR_CLASS(CLASS_NAME)\
class CLASS_NAME : public std::runtime_error {\
    public:\
        explicit CLASS_NAME(const std:: string &what):std::runtime_error(what){}\
};

namespace esdht{
    
    class ESDAsync;
    /**
     * @brief 该类是ESD为前缀的类的基类
     */
    class ESDObject:public std::enable_shared_from_this<ESDObject>{
    // 成员变量
    //
    private:
        
    protected:
        
    public:

        
    // 成员方法
    //
    private:
        // Disable copy construction and assignment for this class and subclasses.
        // 解释参见 http://www.linuxidc.com/Linux/2014-05/101437.htm
        //
        ESDObject(const ESDObject &) = delete;
        ESDObject &operator=(const ESDObject &) = delete;
    protected:
        ESDObject();
        
        
    public:
        
        /**
         * @brief 终止loop，并不会释放loop。如果要释放loop则在stopLoop后添加free(loop)语句。
         */
        static int stopLoop(uv_loop_t *loop);
        
        void addAsync(ESDAsync *async, uv_loop_t *loop, std::function<void (void *data)> func);
        
        virtual ~ESDObject() = 0;
        
        /**
         * @brief Casts the item to the given subclass of ESDObject.
         *
         * @tparam T Subclass of ESDObject.
         */
        template <typename T>
        std::shared_ptr<T> as() {
            static_assert(std::is_base_of<ESDObject, T>::value,
                          "T has to be a subclass of ESDObject");
            
            return std::dynamic_pointer_cast<T>(shared_from_this());
        }
        
        
        
    };

    class ESDAsync : ESDObject{
    public:
        uv_async_t async;
        void *data;
        std::function<void (void *data) > func;
        void sendAsync(void *data);
       
        ~ESDAsync();
    };
    
}//namespace esdobject


#endif /* ESDObject_hpp */
