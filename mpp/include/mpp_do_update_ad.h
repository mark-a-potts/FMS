! -*-f90-*-


!***********************************************************************
!*                   GNU Lesser General Public License
!*
!* This file is part of the GFDL Flexible Modeling System (FMS).
!*
!* FMS is free software: you can redistribute it and/or modify it under
!* the terms of the GNU Lesser General Public License as published by
!* the Free Software Foundation, either version 3 of the License, or (at
!* your option) any later version.
!*
!* FMS is distributed in the hope that it will be useful, but WITHOUT
!* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
!* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
!* for more details.
!*
!* You should have received a copy of the GNU Lesser General Public
!* License along with FMS.  If not, see <http://www.gnu.org/licenses/>.
!***********************************************************************

    subroutine MPP_DO_UPDATE_AD_3D_( f_addrs, domain, update, d_type, ke, flags)
!updates data domain of 3D field whose computational domains have been computed
      integer(i8_kind),         intent(in) :: f_addrs(:,:)
      type(domain2D),             intent(in) :: domain
      type(overlapSpec),          intent(in) :: update
      MPP_TYPE_,                  intent(in) :: d_type  ! creates unique interface
      integer,                    intent(in) :: ke
      integer, optional,          intent(in) :: flags

      MPP_TYPE_ :: field(update%xbegin:update%xend, update%ybegin:update%yend,ke)
      pointer(ptr_field, field)
      integer                     :: update_flags
      type(overlap_type), pointer :: overPtr => NULL()
      character(len=8)            :: text

!equate to mpp_domains_stack
      MPP_TYPE_ :: buffer(size(mpp_domains_stack(:)))
      pointer( ptr, buffer )
      integer :: buffer_pos

!receive domains saved here for unpacking
!for non-blocking version, could be recomputed
      integer,    allocatable :: msg1(:), msg2(:), msg3(:)
      logical :: send(8), recv(8), update_edge_only
      integer :: to_pe, from_pe, pos, msgsize
      integer :: n, l_size, l, m, i, j, k
      integer :: is, ie, js, je, tMe, dir
      integer :: start, start1, start2, index, is1, ie1, js1, je1, ni, nj, total
      integer :: buffer_recv_size, nlist, outunit
      integer :: send_start_pos
      integer :: send_msgsize(MAXLIST)

      outunit = stdout()
      ptr = LOC(mpp_domains_stack)
      l_size = size(f_addrs,1)

      update_flags = XUPDATE+YUPDATE   !default
      if( PRESENT(flags) )update_flags = flags

      update_edge_only = BTEST(update_flags, EDGEONLY)
      recv(1) = BTEST(update_flags,EAST)
      recv(3) = BTEST(update_flags,SOUTH)
      recv(5) = BTEST(update_flags,WEST)
      recv(7) = BTEST(update_flags,NORTH)
      if( update_edge_only ) then
         if( .NOT. (recv(1) .OR. recv(3) .OR. recv(5) .OR. recv(7)) ) then
            recv(1) = .true.
            recv(3) = .true.
            recv(5) = .true.
            recv(7) = .true.
         endif
      else
         recv(2) = recv(1) .AND. recv(3)
         recv(4) = recv(3) .AND. recv(5)
         recv(6) = recv(5) .AND. recv(7)
         recv(8) = recv(7) .AND. recv(1)
      endif
      send    = recv

      if(debug_message_passing) then
         nlist = size(domain%list(:))  
         allocate(msg1(0:nlist-1), msg2(0:nlist-1), msg3(0:nlist-1) )
         msg1 = 0
         msg2 = 0
         msg3 = 0
         do m = 1, update%nrecv
            overPtr => update%recv(m)
            msgsize = 0
            do n = 1, overPtr%count
               dir = overPtr%dir(n)
               if(recv(dir)) then
                  is = overPtr%is(n); ie = overPtr%ie(n)
                  js = overPtr%js(n); je = overPtr%je(n)
                  msgsize = msgsize + (ie-is+1)*(je-js+1)
               end if
            end do
            from_pe = update%recv(m)%pe
            l = from_pe-mpp_root_pe()
            msg2(l) = msgsize
         enddo

         do m = 1, update%nsend
            overPtr => update%send(m)
            msgsize = 0
            do n = 1, overPtr%count
               dir = overPtr%dir(n)
               if(send(dir)) then
                  is = overPtr%is(n); ie = overPtr%ie(n)
                  js = overPtr%js(n); je = overPtr%je(n)
                  msgsize = msgsize + (ie-is+1)*(je-js+1)
               end if
            end do
            l = overPtr%pe - mpp_root_pe()
            msg3(l) = msgsize
         enddo
         call mpp_alltoall(msg3, 1, msg1, 1)

         do m = 0, nlist-1
            if(msg1(m) .NE. msg2(m)) then
               print*, "My pe = ", mpp_pe(), ",domain name =", trim(domain%name), ",from pe=", &
                    domain%list(m)%pe, ":send size = ", msg1(m), ", recv size = ", msg2(m)
               call mpp_error(FATAL, "mpp_do_update: mismatch on send and recv size")
            endif
         enddo
         write(outunit,*)"NOTE from mpp_do_update: message sizes are matched between send and recv for domain " &
                          //trim(domain%name)
         deallocate(msg1, msg2, msg3)
      endif

      !recv
      buffer_pos = 0
      do m = 1, update%nrecv
         overPtr => update%recv(m)
         if( overPtr%count == 0 )cycle
         msgsize = 0
         do n = 1, overPtr%count
            dir = overPtr%dir(n)
            if(recv(dir)) then
               is = overPtr%is(n); ie = overPtr%ie(n)
               js = overPtr%js(n); je = overPtr%je(n)
               msgsize = msgsize + (ie-is+1)*(je-js+1)
            end if
         end do

         msgsize = msgsize*ke*l_size
         if( msgsize.GT.0 )then
            buffer_pos = buffer_pos + msgsize
         end if
      end do ! end do m = 1, update%nrecv
      buffer_recv_size = buffer_pos
      send_start_pos = buffer_pos

      ! send info
      !----------------------------------------------------------------------
      buffer_pos = buffer_recv_size 
      ! pack
      do m = 1, update%nsend
         send_msgsize(m) = 0
         overPtr => update%send(m)
         if( overPtr%count == 0 )cycle
         pos = buffer_pos
         msgsize = 0
         do n = 1, overPtr%count
            dir = overPtr%dir(n)
            if( send(dir) )  msgsize = msgsize + overPtr%msgsize(n)
         enddo
         if( msgsize.GT.0 )then
            msgsize = msgsize*ke*l_size
         end if

         do n = 1, overPtr%count
            dir = overPtr%dir(n)
            if( send(dir) ) then
               tMe = overPtr%tileMe(n)
               is = overPtr%is(n); ie = overPtr%ie(n)
               js = overPtr%js(n); je = overPtr%je(n)
               pos = pos + (ie-is+1)*(je-js+1)*ke*l_size
            endif
         end do ! do n = 1, overPtr%count

         send_msgsize(m) = pos-buffer_pos
         buffer_pos = pos
      end do ! end do m = 1, nsend
 
      !backward communication
      !----------------------------------------------------------------------
      !recv
      buffer_pos = buffer_recv_size
      do m = update%nrecv, 1, -1
         overPtr => update%recv(m)
         if( overPtr%count == 0 )cycle
         pos = buffer_pos
         do n = overPtr%count, 1, -1
            dir = overPtr%dir(n)
            if( recv(dir) ) then
               tMe = overPtr%tileMe(n)
               is = overPtr%is(n); ie = overPtr%ie(n)
               js = overPtr%js(n); je = overPtr%je(n)
               msgsize = (ie-is+1)*(je-js+1)*ke*l_size
               pos = buffer_pos - msgsize
               buffer_pos = pos
               do l=1,l_size  ! loop over number of fields
                  ptr_field = f_addrs(l, tMe)
                  do k = 1,ke
                     do j = js, je
                        do i = is, ie
                           pos = pos + 1
                           buffer(pos) = field(i,j,k)
                        end do
                     end do
                  end do
               end do
            endif
         end do ! do n = 1, overPtr%count
      end do

      !----------------------------------------------------------------------
      buffer_pos = send_start_pos
      do m = 1, update%nsend
         msgsize = send_msgsize(m)
         if(msgsize == 0) cycle
         to_pe = update%send(m)%pe
         call mpp_recv( buffer(buffer_pos+1), glen=msgsize, from_pe=to_pe, block=.FALSE., tag=COMM_TAG_2 )
         buffer_pos = buffer_pos + msgsize
      end do ! end do ist = 0,nlist-1

      !----------------------------------------------------------------------
      !recv
      buffer_pos = 0
      do m = 1, update%nrecv
         overPtr => update%recv(m)
         if( overPtr%count == 0 )cycle
         msgsize = 0
         do n = 1, overPtr%count
            dir = overPtr%dir(n)
            if(recv(dir)) then
               is = overPtr%is(n); ie = overPtr%ie(n)
               js = overPtr%js(n); je = overPtr%je(n)
               msgsize = msgsize + (ie-is+1)*(je-js+1)
            end if
         end do

         msgsize = msgsize*ke*l_size
         if( msgsize.GT.0 )then
            from_pe = overPtr%pe
            mpp_domains_stack_hwm = max( mpp_domains_stack_hwm, (buffer_pos+msgsize) )
            if( mpp_domains_stack_hwm.GT.mpp_domains_stack_size )then
               write( text,'(i8)' )mpp_domains_stack_hwm
               call mpp_error( FATAL, 'MPP_DO_UPDATE: mpp_domains_stack overflow, '// &
                    'call mpp_domains_set_stack_size('//trim(text)//') from all PEs.' )
            end if
            call mpp_send( buffer(buffer_pos+1), plen=msgsize, to_pe=from_pe, tag=COMM_TAG_2 )
            buffer_pos = buffer_pos + msgsize
         end if
      end do ! end do m = 1, update%nrecv

      call mpp_sync_self(check=EVENT_RECV)

      !----------------------------------------------------------------------
      buffer_pos = buffer_recv_size
      do m = 1, update%nsend
         send_msgsize(m) = 0
         overPtr => update%send(m)
         if( overPtr%count == 0 )cycle
         pos = buffer_pos
         msgsize = 0
         do n = 1, overPtr%count
            dir = overPtr%dir(n)
            if( send(dir) )  msgsize = msgsize + overPtr%msgsize(n)
         enddo
         if( msgsize.GT.0 )then
            msgsize = msgsize*ke*l_size
            mpp_domains_stack_hwm = max( mpp_domains_stack_hwm, pos+msgsize )
            if( mpp_domains_stack_hwm.GT.mpp_domains_stack_size )then
               write( text,'(i8)' )mpp_domains_stack_hwm
               call mpp_error( FATAL, 'MPP_START_UPDATE_DOMAINS: mpp_domains_stack overflow, ' // &
                    'call mpp_domains_set_stack_size('//trim(text)//') from all PEs.')
            end if
         end if

         do n = 1, overPtr%count
            dir = overPtr%dir(n)
            if( send(dir) ) then
               tMe = overPtr%tileMe(n)
               is = overPtr%is(n); ie = overPtr%ie(n)
               js = overPtr%js(n); je = overPtr%je(n)
                  select case( overPtr%rotation(n) )
                  case(ZERO)
                     do l=1,l_size  ! loop over number of fields
                        ptr_field = f_addrs(l, tMe)
                        do k = 1,ke
                           do j = js, je
                              do i = is, ie
                                 pos = pos + 1
                                 field(i,j,k)=field(i,j,k)+buffer(pos)
                              end do
                           end do
                        end do
                     end do
                  case( MINUS_NINETY )
                     do l=1,l_size  ! loop over number of fields
                        ptr_field = f_addrs(l, tMe)
                        do k = 1,ke
                           do i = is, ie
                              do j = je, js, -1
                                 pos = pos + 1
                                 field(i,j,k)=field(i,j,k)+buffer(pos)
                              end do
                           end do
                        end do
                     end do
                  case( NINETY )
                     do l=1,l_size  ! loop over number of fields
                        ptr_field = f_addrs(l, tMe)
                        do k = 1,ke
                           do i = ie, is, -1
                              do j = js, je
                                 pos = pos + 1
                                 field(i,j,k)=field(i,j,k)+buffer(pos)
                              end do
                           end do
                        end do
                     end do
                  case( ONE_HUNDRED_EIGHTY )
                     do l=1,l_size  ! loop over number of fields
                        ptr_field = f_addrs(l, tMe)
                        do k = 1,ke
                           do j = je, js, -1
                              do i = ie, is, -1
                                 pos = pos + 1
                                 field(i,j,k)=field(i,j,k)+buffer(pos)
                              end do
                           end do
                        end do
                     end do
               end select
            endif
         end do ! do n = 1, overPtr%count
         send_msgsize(m) = pos-buffer_pos
         buffer_pos = pos
      end do ! end do m = 1, nsend

      call mpp_sync_self()

      return
    end subroutine MPP_DO_UPDATE_AD_3D_
